/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/16 17:37:23 by lazrossi          #+#    #+#             */
/*   Updated: 2018/09/29 19:31:57 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/includes/libft.h"
#include "../includes/asm.h"
#include <unistd.h>

void output_comment(int fd_read)
{
	(void)fd_read;
}

int		check_label(char *label)
{
	int i;

	i = 0;
	if (!(label) || label[0] == 0)
		return (0);
	while (label[i])
	{
		if (!(ft_strchr(LABEL_CHARS, label[i])))
			return (0);
		i++;
	}
	return (1);
}

char	check_1_command(char *command, char match)
{
	if (match & T_DIR && command[0] == DIRECT_CHAR)
	{
		if (command[1] == LABEL_CHAR)
			if (check_label(&command[2]))
				return (DIR_CODE);
		if (ft_isnumber(&command[1]))
			return (DIR_CODE);
	}
	if (match & T_IND)
	{
		if (command[0] == LABEL_CHAR)
			if (check_label(&command[1]))
				return (IND_CODE);
		if (ft_isnumber(&command[0]))
			return (IND_CODE);
	}
	if (match & T_REG)
		if (command[0] == 'r')
			if (ft_isnumber(&command[1]) && ft_atoi(&command[1]) <= REG_NUMBER)
				return (REG_CODE);
	ft_myexit(ft_strjoin(command, " (command) invoked with the wrong argument")); 
	return (0);
}

int		double_separator(char *commands)
{
	int	i;
	int	ret;

	ret = 0;
	i = 0;
	while (commands[i])
	{
		if (commands[i] == SEPARATOR_CHAR)
		{
			if (ret)
				return (ret);
			ret = 1;
		}
		else if (commands[i] > 33)
			ret = 0;
		i++;
	}
	return (ret);
}

int		g_initial_parser[4][3][2] = {
	{{1, 0}, {0, 1}, {3, 0}}, 
	{{1, 1}, {0, 1}, {2, 1}}, 
	{{3, 0}, {0, 1}, {3, 0}}, 
	{{0, 0}, {0, 0}, {0, 0}}
};

int    g_alpha[128] = {
	0,
	0, 0, 0, 0, 0, 0, 0, 0, 1, 3,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 2, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0
};


void	lexe_one_token(t_line *token)
{
	int i;

	int current_state;
	int end_state;

	int start;

	start = 1;
	i = 0;
	current_state = 0;
	while (current_state || start) // && 
	{
		end_state = current_state;
		current_state = g_initial_parser[end_state][g_alpha[(int)token->line[i]]][0];
		i += g_initial_parser[end_state][g_alpha[(int)token->line[i]]][1];
		start = 0;
	}
	// sauvegarder le i quand je rentre a 0 pour savoir a quelle position je commence et sauvegarder les longeurs
	// et en fonction end_state je connaitrai le type
}

void	read_instructions(t_info *info, t_instruction instructions)
{
	int i;

	i = 0;
	(void)instructions;
	(void)info;
	while (i < info->line_nbr)
	{
		lexe_one_token(&(info->line_tokens[i]));
		i++;
	}
}

// check at the beginning if integers are of size 4 (always the case)
int		main(int ac, char **av)
{
	t_fd			fd;
	t_info			info;
	t_instruction	instructions;

	fd.read = -1;
	fd.write = -1;
	instructions = set_instructions(); 
	info.label_info.label_list = NULL;
	info.label_info.label_categories = LABEL_INITIAL_NBR;
	info.label_info.n = 0;
	info.to_write = NULL;
	info.to_write_size = sizeof(info.header);
	info.write_pos = sizeof(info.header);
	malloc_resize_write_size(&info);
	if (ac != 2)
		ft_myexit("You need to pass not more or less than one file to assemble");
	set_name_open_fd(&info, &fd, av[1]);
	read_file(&info, fd);

	store_name_comment(&info, PROG_NAME_LENGTH);
	read_instructions(&info, instructions);
	/*
	check_label_list(&(info.label_info));
	input_labels(&(info.label_info), &info);
	write_file(fd, &info);
	*/
	close(fd.read);
//	sleep(45);
}
