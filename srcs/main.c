/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/16 17:37:23 by lazrossi          #+#    #+#             */
/*   Updated: 2018/09/20 22:48:47 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/includes/libft.h"
#include "../includes/asm.h"
#include <fcntl.h>
#include <stdlib.h>

//this is for the sleep which is tracking leaks
#include <unistd.h>

void output_comment(int fd_read)
{
	(void)fd_read;
}


void	check_1_command(char *command, char match)
{
	// you should return a function to check DIRECT_CHAR / LABEL_CHAR / etc
	if (match & T_DIR)
	{
	if (command[0] == DIRECT_CHAR)
			if (command[1] == LABEL_CHAR)
				(label_list(&command[2], 0));
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

void	check_instruction_arguments(char *commands, int i, t_instruction instructions)
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
		check_1_command(all_commands[(int)j], instructions.instruct_arg[i][(int)j + 1]);
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

char	*clean_instructions(char *line, t_instruction instruction)
{
	char	**line_split;
	int		instruction_passed;
	int		i;
	int		j;

	i = 0;
	line_split = NULL;
	instruction_passed = 0;
	while (line[i] && !instruction_passed)
	{
		j = -1;
		while (!instruction_passed && ++j < INSTRUCT_NBR)
		{
			if (!(ft_memcmp(&line[i], instruction.names[j], ft_strlen(instruction.names[j]))))
			{
				if (line[i + ft_strlen(instruction.names[j]) + 1])
					i += ft_strlen(instruction.names[j]) + 1;
				else
					ft_myexit("incomplete instruction");
				instruction_passed = 1;
			}
		}
		i++;
	}
	return (line = check_white_space_redudancy(line, i));
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

int		check_if_label(char *line)
{
	char	**tmp;
	char	*to_check;
	
	if (!(tmp = ft_split_whitespaces(line)))
		ft_myexit("error in ft_split_whitespaces");
	to_check = tmp[0];
	if (to_check[ft_strlen(to_check) - 1] == LABEL_CHAR)
	{
		to_check[ft_strlen(to_check) - 1] = 0;
		label_list(to_check, 1);
		ft_tabdel((void***)&tmp);
		return (1);
	}
	ft_tabdel((void***)&tmp);
	return (0);
}

#include <unistd.h>
void	check_instructions(char *line, t_instruction instruction)
{
	char	**line_split;
	int		i;
	int		label;

	i = 0;
	label = 0;
	if ((label = check_if_label(line)))
	{
		while (line[i] != LABEL_CHAR)
			i++;
		i++;
	}
	line = clean_instructions(&line[i], instruction);
	if (!(line_split = ft_split_whitespaces(line)))
		ft_myexit("error in ft_split");
	if (label && !line_split[1])
		return (ft_tabdel((void***)&line_split));
	i = 0;
	while (ft_strcmp(line_split[label], instruction.names[i]) && i < INSTRUCT_NBR)
		i++;
	if (i == INSTRUCT_NBR)
		ft_myexit(ft_strjoin("bad instruction name : ", line_split[label]));
	check_instruction_arguments(line_split[1 + label], i, instruction);
	ft_tabdel((void***)&line_split);
}

void	read_instructions(t_info *info)
{
	while (info->file_read[info->file_lines_nbr])
	{
		if (ft_strlen(info->file_read[info->file_lines_nbr]) > 0)
		{
			if (!(cut_comment(&(info->file_read[info->file_lines_nbr]))))
				return ;
			check_instructions(info->file_read[info->file_lines_nbr], info->instructions);
		}
		info->file_lines_nbr++;
	}
}

void	set_name_open_fd(t_info *info, t_fd *fd, char *to_open)
{
	ft_memcpy(info->file_name, to_open, ft_strlen(to_open));
	ft_memcpy(&(info->file_name[ft_strlen(to_open) - 1]), "cor", 3);
	fd->read = open(to_open, O_RDONLY);
	if (fd->read <= 0)
		ft_myexit("Open error");
}

char	**file_lines(int nbr, char	**file_lines)
{
	char	**new;
	int		i;

	new = NULL;
	i = 0;
	if (!(new = malloc(sizeof(char *) * nbr)))
		ft_myexit("malloc error in file_lines");
	while (i < nbr)
	{
		new[i] = NULL;
		i++;
	}
	if (file_lines)
	{
		i = 0;
		while (i < nbr / 2)
		{
			new[i] = file_lines[i];
			i++;
		}
		ft_tabdel((void***)&file_lines);
	}
	return (file_lines = new);
}

void	read_file(t_info *info, t_fd fd)
{
	int		gnl_ret;
	char	*buf;
	int		i;

	i = 0;
	info->file_read = NULL;
	info->file_lines_nbr = FILE_INITIAL_SIZE;
	info->file_read = file_lines(FILE_INITIAL_SIZE, info->file_read);
	while ((gnl_ret = get_next_line(fd.read, &buf, '\n')) > 0)
	{
		if (i >= info->file_lines_nbr)
		{
			info->file_lines_nbr *= 2;
			info->file_read = file_lines(info->file_lines_nbr, info->file_read);
		}
		(info->file_read)[i] = buf;
		i++;
	}
	if (gnl_ret < 0)
		ft_myexit("get_next_line error");
}

//	fd.write = open(info.file_name, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);

int		main(int ac, char **av)
{
	t_fd	fd;
	t_info	info;

	fd.read = -1;
	fd.write = -1;
	info.instructions = set_instructions(); 
	if (ac != 2)
		ft_myexit("You need to pass not more or less than one file to assemble");
	set_name_open_fd(&info, &fd, av[1]);
	read_file(&info, fd);
	info.file_lines_nbr = 0;
	store_name_comment(&info, PROG_NAME_LENGTH);
	ft_printf("%sis name %s is comment\n", info.header.prog_name, info.header.comment);
	read_instructions(&info);
	ft_printf("[[red]][[bold]]YOU ARE DONE PARSING !!![[end]]");
	sleep(45);
}
