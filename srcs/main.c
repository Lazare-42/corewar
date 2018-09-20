/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/16 17:37:23 by lazrossi          #+#    #+#             */
/*   Updated: 2018/09/20 18:26:56 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/includes/libft.h"
#include "../includes/asm.h"
#include <fcntl.h>

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

void	check_instruction_arguments(char *commands, int i, t_instruction instructions)
{
	char	**all_commands;
	char	command_size;
	char	j;

	j = -1;
	command_size = 0;
	all_commands = NULL;
	if (!(all_commands = ft_split_char(commands, SEPARATOR_CHAR)))
		ft_myexit("ft_split failed. Use SEPARATOR_CHAR to distinguish instructions");
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


char	**split_instructions(char *line, t_instruction instruction, int start)
{
	char	**line_split;
	int		instruction_passed;
	int		i;
	int		j;

	i = 0;
	if (start)
	{
		while (line[i] != LABEL_CHAR)
			i++;
		i++;
	}
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
	while (line[i])
	{
		while (instruction_passed && i > 0 && line[i + 1] && line[i] == ' ') 
			line = delete_redudant_char(line, i);
		i++;
	}
	//ft_printf("[[blue]]%s\n[[end]]", line);
	//sleep(45);
	if (!(line_split = ft_split_whitespaces(line)))
		ft_myexit("error in ft_split");
	return (line_split);
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
	int		start;

	i = 0;
	start = 0;
	if (!(cut_comment(&line)))
		return ;
	if (check_if_label(line))
		start = 1;
	line_split = split_instructions(line, instruction, start);
	if (start && !line_split[1])
	{
		ft_tabdel((void***)&line_split);
		return ;
	}
	while (ft_strcmp(line_split[start], instruction.names[i]) && i < INSTRUCT_NBR)
		i++;
	if (i == INSTRUCT_NBR)
	{
		// You should here check if what you got is a label followed by instructions
		ft_myexit(ft_strjoin("bad instruction name : ", line_split[start]));
	}
	check_instruction_arguments(line_split[1 + start], i, instruction);
	ft_tabdel((void***)&line_split);
}

void	read_instructions(int fd_read, int fd_write)
{
	int				ret;
	char			*buf;
	t_instruction	instructions;

	buf = NULL;
	(void)fd_write;
	instructions = set_instructions();
	while ((ret = get_next_line(fd_read, &buf, '\n') > 0))
	{
		if (ft_strlen(buf) > 0)
			check_instructions(buf, instructions);
		ft_memdel((void**)&buf);
	}
	if (ret < 0)
		ft_myexit("get_next_line error");
}

int		main(int ac, char **av)
{
	int		fd_read;
	int		fd_write;
	int		strlen;
	char	name[256];

	if (ac != 2)
		ft_myexit("You need to pass not more or less than one file to assemble");
	strlen = ft_strlen(av[1]);
	ft_memcpy(name, av[1], strlen);
	ft_memcpy(&name[strlen - 1], "cor", 3); fd_read = open(av[1], O_RDONLY);
	fd_write = open(name, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
	if (fd_read <= 0 || fd_write <= 0)
		ft_myexit("Open error");
	// you should check instructions and name and comment before inputing
//	input_magic(fd_write);
	output_name_comment(fd_read, fd_write, PROG_NAME_LENGTH);
	read_instructions(fd_read, fd_write);
	ft_printf("[[red]][[bold]]YOU ARE DONE PARSING !!![[end]]");
	sleep(45);
}
