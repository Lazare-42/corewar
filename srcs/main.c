/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/16 17:37:23 by lazrossi          #+#    #+#             */
/*   Updated: 2018/09/23 16:46:08 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/includes/libft.h"
#include "../includes/asm.h"

//this is for the sleep which is tracking leaks
#include <unistd.h>

void output_comment(int fd_read)
{
	(void)fd_read;
}


void	check_1_command(t_info *info, char *command, char match)
{
	// you should return a function to check DIRECT_CHAR / LABEL_CHAR / etc
	if (match & T_DIR)
	{
	if (command[0] == DIRECT_CHAR)
			if (command[1] == LABEL_CHAR)
				(label_list(&(info->label_info), &command[2], 0));
					return ;
		if (ft_isdigit(command[1]) || (command[1] == '-' && ft_isdigit(command[2]))) //ft_atoi(command[1]);
			return ; 
	}
	if (match & T_IND)
	{
		if (command[0] == LABEL_CHAR)
			return ;
		// ok here you need to find the label's adress
		if (ft_isdigit(command[0]) || (command[0] == '-' && ft_isdigit(command[1])))
			return ;
		// ok here you need to check if the adresse indirectly pointed to is ...a label ?
	}
	if (match & T_REG)
		if (command[0] == 'r')
			if (ft_atoi(&command[1]) <= REG_NUMBER)
				return ;
	ft_myexit(ft_strjoin(command, " (command) invoked with the wrong argument")); 
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

	j = -1;
	command_size = 0;
	all_commands = NULL;
	if (double_separator(commands) || !(all_commands = ft_split_char(commands, SEPARATOR_CHAR)))
		ft_myexit("ft_split failed. Use only one SEPARATOR_CHAR to distinguish instructions");
	while (all_commands[(int)command_size])
		command_size++;
	if (command_size != instructions.instruct_arg[i][0])
		ft_myexit(ft_strjoin("incorrect argument number passed to instruction : ", instructions.names[i]));
	while (++j < command_size)
		check_1_command(info, all_commands[(int)j], instructions.instruct_arg[i][(int)j + 1]);
	ft_tabdel((void***)&all_commands);
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

char	*check_white_space_redudancy(char *line, int from)
{
	while (line[from])
	{
		while (from > 0 && line[from + 1] && line[from] == ' ') 
			line = delete_redudant_char(line, from);
		from++;
	}
	return (line);
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
				info->file_read[info->file_lines_nbr] = check_white_space_redudancy(info->file_read[info->file_lines_nbr], i);
				return ;
			}
		}
		i++;
	}
}

int		cut_comment(char **line)
{
	int i;
	int	line_to_treat;

	i = 0;
	line_to_treat = 0;
	while ((*line)[i])
	{
		if ((*line)[i] == '#')
		{
			(*line)[i] = 0;
			break ;
		}
		else if ((*line)[i] > 33)
			line_to_treat = 1;
		i++;
	}
	return (line_to_treat);
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
		label_list(&(info->label_info), to_check, 1);
		ft_tabdel((void***)&tmp);
		return (1);
	}
	ft_tabdel((void***)&tmp);
	return (0);
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

void	read_instructions(t_info *info, t_instruction instructions)
{
	info->file_lines_nbr++;
	while (info->file_read[info->file_lines_nbr])
	{
		if (ft_strlen(info->file_read[info->file_lines_nbr]) > 0)
		{
			if ((cut_comment(&(info->file_read[info->file_lines_nbr]))))
				check_instructions(info, instructions);
		}
		info->file_lines_nbr++;
	}
}

//	fd.write = open(info.file_name, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);

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
	if (ac != 2)
		ft_myexit("You need to pass not more or less than one file to assemble");
	set_name_open_fd(&info, &fd, av[1]);
	read_file(&info, fd);
	info.file_lines_nbr = 0;
	store_name_comment(&info, PROG_NAME_LENGTH);
	read_instructions(&info, instructions);
	print_label_list(&(info.label_info));
	ft_printf("[[red]][[bold]]DONE PARSING[[end]]");
	sleep(45);
}
