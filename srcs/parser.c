/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/08 09:25:14 by lazrossi          #+#    #+#             */
/*   Updated: 2018/10/08 20:45:13 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/asm.h"
#include "../libft/includes/libft.h"

static t_op    g_op_tab[17] =
{
	{"", 0, {0}, 0, 0, "first void line", 0, 0},
	{"live", 1, {T_DIR}, 1, 10, "alive", 0, 0},
	{"ld", 2, {T_DIR | T_IND, T_REG}, 2, 5, "load", 1, 0},
	{"st", 2, {T_REG, T_IND | T_REG}, 3, 5, "store", 1, 0},
	{"add", 3, {T_REG, T_REG, T_REG}, 4, 10, "addition", 1, 0},
	{"sub", 3, {T_REG, T_REG, T_REG}, 5, 10, "soustraction", 1, 0},
	{"and", 3, {T_REG | T_DIR | T_IND, T_REG | T_IND | T_DIR, T_REG}, 6, 6,
		"et (and  r1, r2, r3   r1&r2 -> r3", 1, 0},
	{"or", 3, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}, 7, 6,
		"ou  (or   r1, r2, r3   r1 | r2 -> r3", 1, 0},
	{"xor", 3, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}, 8, 6,
		"ou (xor  r1, r2, r3   r1^r2 -> r3", 1, 0},

	{"zjmp", 1, {T_DIR}, 9, 20, "jump if zero", 0, 1},
	{"ldi", 3, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}, 10, 25,
		"load index", 1, 1},
	{"sti", 3, {T_REG, T_REG | T_DIR | T_IND, T_DIR | T_REG}, 11, 25,
		"store index", 1, 1},
	{"fork", 1, {T_DIR}, 12, 800, "fork", 0, 1},

	{"lld", 2, {T_DIR | T_IND, T_REG}, 13, 10, "long load", 1, 0},

	{"lldi", 3, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}, 14, 50,
		"long load index", 1, 1},
	{"lfork", 1, {T_DIR}, 15, 1000, "long fork", 0, 1},
	{"aff", 1, {T_REG}, 16, 2, "aff", 1, 0},
};

int		check_label(char *label, int size)
{
	int i;

	i = 0;
	if (!(label) || label[0] == 0)
		return (0);
	while (i < size)
	{
		if (!(ft_strchr(LABEL_CHARS, label[i])))
			return (0);
		i++;
	}
	return (1);
}

int		is_num(char *to_check, int size)
{
	int i;

	i = 0;
	while (i < size)
	{
		if (to_check[i] < '0' || i > '9')
			return (0);
		i++;
	}
	return (1);
}

char	check_1_command(char *command, char match, int size)
{
	if (match & T_DIR && command[0] == DIRECT_CHAR)
	{
		if (command[1] == LABEL_CHAR)
			if (check_label(&command[2], size))
				return (DIR_CODE);
		if (is_num(&command[1], size - 1))
			return (DIR_CODE);
	}
	if (match & T_IND)
	{
		if (command[0] == LABEL_CHAR)
			if (check_label(&command[1], size))
				return (IND_CODE);
		if (is_num(&command[0], size - 1))
			return (IND_CODE);
	}
	if (match & T_REG)
		if (command[0] == 'r')
			if (is_num(&command[1], size - 1) && ft_atoi(&command[1]) <= REG_NUMBER)
				return (REG_CODE);
	// info->error  = 1;
	// save error
	//ft_myexit("wrong instruction info");
	return (0);
}

void	check_one_function(unsigned char function_nbr, t_token function_token, t_info *info)
{
	unsigned char	command_binary;
	unsigned char	one_command;
	unsigned int	cmd_size;
	unsigned int	i;

	command_binary = 0;
	i = 0;
	cmd_size = 0;
	while (i < g_op_tab[function_nbr].parameters_nbr)
	{
		one_command = check_1_command(&info->file[function_token.token[i + 1][0]], g_op_tab[function_nbr].parameter_types[i], function_token.token[i + 1][1] - function_token.token[i + 1][0]);
		if (one_command == REG_CODE)
			cmd_size += T_REG;
		else if (one_command == IND_CODE)
			cmd_size += T_IND;
		else if (one_command == DIR_CODE)
			cmd_size += (g_op_tab[function_nbr].double_dir_size) ? T_DIR * 2 : T_DIR;
		command_binary |=  one_command << (6 - i * 2);
		i++;
	}
	cmd_size += (command_binary << 2 || function_nbr == 16) ? 2 : 1;
	if ((info->write_pos + cmd_size) > info->to_write_size)
		malloc_resize_write_size(info);
	(info->to_write)[info->write_pos] = function_nbr;
	if (command_binary << 2 || function_nbr == 16)
	{
		(info->to_write)[info->write_pos] = command_binary;
		info->write_pos += 1;
	}
	info->write_pos += 1;
}

void	parse_functions(t_info *info)
{
	unsigned int	i;
	unsigned char	j;
	unsigned int	memcmp_siz;

	i = 0;
	while (i < info->token_nbr)
	{
		j = 1;
		memcmp_siz = info->tokens[i].token[0][1] - info->tokens[i].token[0][0];
		if (info->tokens[i].func_nbr == FUNCTION)
		{
			while (j < 17 && (memcmp_siz != ft_strlen(g_op_tab[j].name) || ft_memcmp(&info->file[info->tokens[i].token[0][0]], g_op_tab[j].name, memcmp_siz)))
				j++;
			if (j == 17)
			{
				// info->error  = 1;
				// save error
			}
			else
				check_one_function(j, info->tokens[i], info);
		}
		i++;
	}
}
