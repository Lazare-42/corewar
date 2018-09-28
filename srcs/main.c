/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/16 17:37:23 by lazrossi          #+#    #+#             */
/*   Updated: 2018/09/28 22:30:12 by lazrossi         ###   ########.fr       */
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

void	clean_instructions(t_info *info, t_instruction instruction, int i)
{
	int		j;

	while (info->file_read[info->file_lines_nbr][i])
	{
		j = -1;
		while (++j < INSTRUCT_NBR)
		{
			if (!(ft_memcmp(&info->file_read[info->file_lines_nbr][i], instruction.names[j], ft_strlen(instruction.names[j]))))
			{
				if (info->file_read[info->file_lines_nbr][i + ft_strlen(instruction.names[j]) + 1])
					i += ft_strlen(instruction.names[j]) + 1;
				else
					ft_myexit("incomplete instruction");
				i++;
				return ;
			}
		}
		i++;
	}
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

void	check_instruction_arguments(t_info *info, char *commands, int i, t_instruction instructions)
{
	char	**all_commands;
	char	command_size;
	char	j;
	unsigned char command_binary;

	j = -1;
	command_size = 0;
	command_binary = 0;
	all_commands = NULL;
	ft_printf("%sis commands\n", commands);
	if (!(all_commands = ft_split_char(commands, SEPARATOR_CHAR)))
		ft_myexit("ft_split failed. Use only one SEPARATOR_CHAR to distinguish instructions");
	while (all_commands[(int)command_size])
		command_size++;
	if (command_size != instructions.instruct_arg[i][0])
		ft_myexit(ft_strjoin("incorrect argument number passed to instruction : ", instructions.names[i]));
	while (++j < command_size)
		command_binary |= check_1_command(all_commands[(int)j],
			 	instructions.instruct_arg[i][(int)j + 1]) << (6 - j * 2);
	write_instruction_info(info, command_binary, i + 1);
	write_instruction(info, command_binary, all_commands, i + 1);
	ft_tabdel((void***)&all_commands);
}

void	cut_comment(char **line)
{
	int i;

	i = 0;
	while ((*line)[i])
	{
		if ((*line)[i] == '#')
		{
			(*line)[i] = 0;
			break ;
		}
		i++;
	}
}

int		check_if_label(char *line, t_info *info)
{
	char	**tmp;
	char	*to_check;

	if (!(tmp = ft_split_whitespaces(line)))
		ft_myexit("error in ft_split_whitespaces");
	to_check = tmp[0];
	if (to_check[ft_strlen(to_check) - 1] == LABEL_CHAR)
	{
		to_check[ft_strlen(to_check) - 1] = 0;
		label_list(&(info->label_info), new_label(to_check, info->write_pos, 0, 0));
		ft_tabdel((void***)&tmp);
		return (1);
	}
	ft_tabdel((void***)&tmp);
	return (0);
}

void	check_label_presence(t_info *info)
{
	char	*label_pos;

	label_pos = NULL;
	if (!(label_pos = ft_strchr(info->file_read[info->file_lines_nbr], ':')))
		return ;
}

void	check_instructions(t_info *info, t_instruction instructions)
{
	char	**instruct_split;
	int		i;
	int		label;

	i = 0;
	label = 0;
	instruct_split = NULL;

	if ((label = check_if_label(info->file_read[info->file_lines_nbr], info)))
		while (info->file_read[info->file_lines_nbr][i] != LABEL_CHAR)
			i++;
	if (label)
		i++;

	clean_instructions(info, instructions, i);
	if (!(instruct_split = ft_split_whitespaces(info->file_read[info->file_lines_nbr])))
		ft_myexit("error in ft_split");
	if ((label && !instruct_split[1]) || !instruct_split[0])
		return (ft_tabdel((void***)&instruct_split));
	i = 0;
	while (ft_strcmp(instruct_split[label], instructions.names[i]) && i < INSTRUCT_NBR)
		i++;
	if (i == INSTRUCT_NBR)
		ft_myexit(ft_strjoin("bad info->instructions name : ", instruct_split[label]));
	check_instruction_arguments(info, instruct_split[1 + label], i, instructions);
	ft_tabdel((void***)&instruct_split);
}

char	*delete_redudant_char(char *line, int i)
{
	int j;

	i++;
	while (line[i])
	{
		j = i - 1;
		line[j] = line[i];
		i++;
	}
	line[j + 1] = 0;
	return (line);
}

char	*check_white_space_redudancy(char *line)
{
	int i;
	int token;	

	i = 0;
	token = 0;
	while (line[i])
	{
		while (line[i] && line[i] < 33) 
			line = delete_redudant_char(line, i);
		if (line[i] && line[i] == ',' && i - 1 > 0)
		{
			line = delete_redudant_char(line, i - 1);
			token = 1;
		}
		while (line[i] && line[i] >= 33)
		{
			token = 0;
			if (line[i] == ',')
				token = 1;
			i++;
		}
		if (line[i] && !token)
			i++;
	}
	return (line);
}

void	read_instructions(t_info *info, t_instruction instructions)
{
	info->file_lines_nbr++;
	while (info->file_read[info->file_lines_nbr])
	{
		cut_comment(&(info->file_read[info->file_lines_nbr]));
		ft_printf("%s\n%n", info->file_read[info->file_lines_nbr]);
		check_white_space_redudancy(info->file_read[info->file_lines_nbr]);
		ft_printf("%s\n%n", info->file_read[info->file_lines_nbr]);
		if (info->file_read[info->file_lines_nbr][0])
				check_instructions(info, instructions);
		info->file_lines_nbr++;
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
	info.file_lines_nbr = 0;
	store_name_comment(&info, PROG_NAME_LENGTH);
	read_instructions(&info, instructions);
	check_label_list(&(info.label_info));
	input_labels(&(info.label_info), &info);
	write_file(fd, &info);
	close(fd.read);
//	sleep(45);
}
