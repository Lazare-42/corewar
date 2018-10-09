/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/08 09:25:14 by lazrossi          #+#    #+#             */
/*   Updated: 2018/10/09 22:56:21 by lazrossi         ###   ########.fr       */
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
	if (to_check[i] == '-')
		i++;
	while (i < size)
	{
		if (to_check[i] < '0' || to_check[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

char	check_1_cmd(char *cmd, char match, int size)
{		
	if (match & T_DIR && cmd[0] == DIRECT_CHAR)
	{
		if (cmd[1] == LABEL_CHAR)
			if (check_label(&cmd[2], size - 2))
				return (DIR_CODE);
		if (is_num(&cmd[1], size - 1))
			return (DIR_CODE);
	}
	if (match & T_IND)
	{
		if (cmd[0] == LABEL_CHAR)
			if (check_label(&cmd[1], size - 1))
				return (IND_CODE);
		if (is_num(&cmd[0], size - 1))
			return (IND_CODE);
	}
	if (match & T_REG)
		if (cmd[0] == 'r')
			if (is_num(&cmd[1], size - 1) && ft_atoi(&cmd[1]) <= REG_NUMBER)
				return (REG_CODE);
	// info->error  = 1;
	// save error
	// incorrect argument passed to function 
	//ft_myexit("wrong instruction info");
	return (0);
}

int	check_one_function(unsigned char func_nbr, t_token function_token, t_info *info, t_cmd *cmd_params)
{
	unsigned int	i;

	i = 0;
	cmd_params->cmd_binary = 0;
	cmd_params->cmd_size = 0;
	while (i < g_op_tab[func_nbr].parameters_nbr)
	{
		if (!(cmd_params->one_cmd = check_1_cmd(&info->file[function_token.token[i + 1][0]], g_op_tab[func_nbr].parameter_types[i], function_token.token[i + 1][1] - function_token.token[i + 1][0])))
			return (0);
		if (cmd_params->one_cmd == REG_CODE)
			cmd_params->cmd_size += T_REG;
		else if (cmd_params->one_cmd == IND_CODE)
			cmd_params->cmd_size += T_IND;
		else if (cmd_params->one_cmd == DIR_CODE)
			cmd_params->cmd_size += (g_op_tab[func_nbr].double_dir_size) ? T_DIR * 2 : T_DIR;
		cmd_params->cmd_binary |=  cmd_params->one_cmd << (6 - i * 2);
		i++;
	}
	cmd_params->cmd_size += (cmd_params->cmd_binary << 2 || func_nbr == 16) ? 2 : 1;
	if ((info->write_pos + cmd_params->cmd_size) > info->to_write_size)
		malloc_resize_write_size(info);
	return (1);
}

void	write_one_reg(t_info *info, int instruction_pos)
{
	char	reg;

	reg = ft_atoi(&info->file[instruction_pos + 1]);
	ft_memcpy((void*)&info->to_write[info->write_pos], &reg, sizeof(char));
	info->write_pos += T_REG;
}

void	write_one_indirect(t_info *info, int instruction_pos, int instruction_len)
{
	short	indirection;

	if (info->file[instruction_pos] != LABEL_CHAR)
	{
		indirection = ft_atoi(&info->file[instruction_pos]);
		if (ft_check_little_endianness())
			indirection = little_endian_to_big(indirection, sizeof(short));
		ft_memcpy((void*)&info->to_write[info->write_pos], &indirection, (T_IND / 2));
	}
	else
	{
		label_list(info, new_label(instruction_pos + 1, instruction_len - 1, info->cmd_begin_pos, info->write_pos,  T_IND / 2));
	}
	info->write_pos += T_IND / 2;
}

void	write_one_direct(t_info *info, unsigned int instruction_pos, unsigned int instruction_len, unsigned char func_nbr)
{
	int		reg_ind;

	if (info->file[instruction_pos + 1] == LABEL_CHAR)
	{
		label_list(info, new_label(instruction_pos + 2, instruction_len - 2, info->cmd_begin_pos, info->write_pos, (func_nbr < 8 || func_nbr == 13) ? T_DIR * 2 : T_DIR));
	}
	else
	{
		reg_ind = ft_atoi(&info->file[instruction_pos + 1]);
		if (ft_check_little_endianness())
			reg_ind = little_endian_to_big(reg_ind, (func_nbr < 8 || func_nbr == 13) ? T_DIR * 2 : T_DIR);
		ft_memcpy((void*)&info->to_write[info->write_pos], &reg_ind, (func_nbr < 8 || func_nbr == 13) ? T_DIR * 2 : T_DIR );
	}
	info->write_pos += (func_nbr < 8 || func_nbr == 13) ? T_DIR * 2 : T_DIR;
}

void	write_one_function(unsigned char func_nbr, t_token function_token, t_info *info, unsigned char cmd_binary)
{
	int read_command_binary;

	read_command_binary = 6;
	(info->to_write)[info->write_pos] = func_nbr;
	info->cmd_begin_pos = info->write_pos;
	info->write_pos += 1;
	if ((char)(cmd_binary << 2) || func_nbr == 16)
	{
		(info->to_write)[info->write_pos] = cmd_binary;
		info->write_pos += 1;
	}
	while ((cmd_binary>> read_command_binary) & 3 && read_command_binary >= 2)
	{
		if (((cmd_binary >> read_command_binary) & 3) == REG_CODE)
			write_one_reg(info, function_token.token[1 + 3 - read_command_binary / 2][0]);
		if (((cmd_binary >> read_command_binary) & 3) == IND_CODE)
			write_one_indirect(info, function_token.token[1 + 3 - read_command_binary / 2][0], function_token.token[1 + 3 - read_command_binary / 2][1] - function_token.token[1 + 3 - read_command_binary / 2][0]);
		if (((cmd_binary >> read_command_binary) & 3) == DIR_CODE)
			write_one_direct(info, function_token.token[1 + 3 - read_command_binary / 2][0], function_token.token[1 + 3 - read_command_binary / 2][1] - function_token.token[1 + 3 - read_command_binary / 2][0], func_nbr);
		read_command_binary -= 2;
	}
}

void	parse_functions(t_info *info)
{
	unsigned int	i;
	unsigned char	func_nbr;
	unsigned int	memcmp_siz;
	t_cmd			cmd_params;

	i = 0;
	while (i < info->token_nbr)
	{
		func_nbr = 1;
		memcmp_siz = info->tokens[i].token[0][1] - info->tokens[i].token[0][0];
		if (info->tokens[i].func_nbr == FUNCTION)
		{
			while (func_nbr < MAX_FUNC_NBR && (memcmp_siz != ft_strlen(g_op_tab[func_nbr].name) || ft_memcmp(&info->file[info->tokens[i].token[0][0]], g_op_tab[func_nbr].name, memcmp_siz)))
				func_nbr++;
			if (func_nbr == MAX_FUNC_NBR)
			{
				// error : unknown function
				// info->error  = 1;
				// save error
			}
			else
			{
				if (check_one_function(func_nbr, info->tokens[i], info, &cmd_params))
				{
					write_one_function(func_nbr, info->tokens[i], info, cmd_params.cmd_binary); 
				}
			}
		}
		else
			label_list(info, new_label(info->tokens[i].token[0][0], info->tokens[i].token[0][1] - info->tokens[i].token[0][0], info->write_pos, 0, 0));
		i++;
	}
}
