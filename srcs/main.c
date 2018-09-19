/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/16 17:37:23 by lazrossi          #+#    #+#             */
/*   Updated: 2018/09/19 12:32:17 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/includes/libft.h"
#include "../includes/asm.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

void output_comment(int fd_read)
{
	(void)fd_read;
}

int		little_endian_to_big(int src)
{
	unsigned char	tmp[4];
	unsigned char	*number;
	int				ret;

	number = (void*)&src;
	tmp[0] = number[3];
	tmp[1] = number[2];
	tmp[2] = number[1];
	tmp[3] = number[0];
	ft_memcpy(&ret, tmp, 4);
	return (ret);
}

void	convert_to_bytecode(char **result, void *to_convert, int name_or_comment)
{
	int				size;
	int				j;
	int				i;
	unsigned char	tmp;
	unsigned char	*str;

	str = to_convert;
	size = (name_or_comment) ? name_or_comment * 8 : ft_strlen((char*)str) * 8;
	(*result) = NULL;
	if (!((*result) = malloc(sizeof(char) * size)))
		ft_myexit("malloc error");
	ft_memset((*result), 0, size);
	i = 0;
	while ((name_or_comment && i < name_or_comment) || (str[i] && !name_or_comment))
	{
		j = 7;
		tmp = str[i];
		while (tmp)
		{
			(*result)[i * 8 + j] = tmp % 2;
			tmp /= 2;
			j--;
		}
		i++;
	}
}

void	output_name_comment(int fd_read, int fd_write, int which)
{
	char	*buf;
	char	**split;
	char	name[which];
	int		i;

	buf = NULL;
	split = NULL;
	i = 7;
	if (get_next_line(fd_read, &buf, '\n') <= 0)
		ft_myexit("You passed an empty or incomplete file");
	if (!(split = (ft_split_char(buf, '\"'))) || !split[1])
		ft_myexit("malloc error or invalid split");
	if ((ft_strlen(split[1]) > PROG_NAME_LENGTH && which == PROG_NAME_LENGTH) || (ft_strlen(split[1]) > COMMENT_LENGTH && which == COMMENT_LENGTH))
		ft_myexit("Your champion's name or your comment is too long");
	ft_memdel((void**)&buf);
	ft_memset(name, 0, which);
	ft_memcpy(name, split[1], ft_strlen(split[1]));
	write(fd_write, name, which);
	write(fd_write, "\0\0\0\0\0\0\0\0", 8);
	ft_tabdel((void***)&split);
	if (which == PROG_NAME_LENGTH)
		output_name_comment(fd_read, fd_write, COMMENT_LENGTH); 
}

void	printbits(int nbr)
{
	int see;

	see = 1;
	int i = 0;
	while (i < 32)
	{
		if (see << i & nbr)
			ft_printf("1");
		else
			ft_printf("0");
		if (i == 7 || i == 15 || i == 23)
			ft_printf(" ");
		i++;
	}
	ft_printf("\n");
}

void	input_magic(int fd_write)
{
	int				magic_nbr;

	magic_nbr = COREWAR_EXEC_MAGIC;
	if (ft_check_endianness())
		magic_nbr = little_endian_to_big(magic_nbr);
	write(fd_write, &magic_nbr, 4);
}

t_label		*new_label(char *name, int is_anchor)
{
	t_label *new;

	if (!(new = malloc(sizeof(t_label))))
		ft_myexit("malloc error");
	new->name = NULL;
	if (!(new->name = ft_strdup(name)))
		ft_myexit("malloc error");
	new->next = NULL;
	new->position = 0;
	new->anchor = (is_anchor) ? 1 : 0;
	return (new);
}

t_label		*label_list(char	*label, int is_anchor)
{
	static t_label	*label_list = NULL;
	t_label			*new;
	t_label			*tmp;

	tmp = label_list;
	ft_printf("[[blue]]this is label %s\n[[end]]", label);
	if (label)
		new = new_label(label, is_anchor);
	if (!label_list)
		return (label_list = new);
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	return (label_list);
}

void	check_1_command(char *command, char match)
{
	// you should return a function to check DIRECT_CHAR / LABEL_CHAR / etc
	ft_printf("checking : %s ", command);
	if (match & T_DIR)
		ft_printf(" match & T_DIR");
	if (match & T_IND)
		ft_printf(" match & T_IND");
	if (match & T_REG)
		ft_printf(" match & T_REG");
	ft_printf("\n");
	if (match & T_DIR)
	{
		if (command[0] == DIRECT_CHAR)
		{
			if (command[1] == LABEL_CHAR)
			{
				(label_list(&command[2], 0));
				return ;
			}
			if (ft_isdigit(command[1])) //ft_atoi(command[1]);
				return ; 
		}
	}
	if (match & T_IND)
	{
		if (command[0] == LABEL_CHAR)
		{
			ft_printf("[[yellow]]Indirection %s\n[[end]]", command);
			return ;
		}
		// ok here you need to find the label's adress
	//	if (ft_atoi(command))
	//		return ;
		// ok here you need to check if the adresse indirectly pointed to is ...a label ?
	}
	if (match & T_REG)
	{
		ft_printf("[[cyan]]register %s\n[[end]]", command);
		if (command[0] == 'r')
			if (ft_atoi(&command[1]) <= REG_NUMBER) return ;
	}
	ft_myexit(ft_strjoin(command, " (command) invoked with the wrong argument")); 
}

void	check_instruction_arguments(char *commands, int i, t_instruction instructions)
{
	char	**all_commands;
	char	command_size;
	char	j;

	j = 0;
	command_size = 0;
	all_commands = NULL;
	if (!(all_commands = ft_split_char(commands, SEPARATOR_CHAR)))
		ft_myexit("ft_split failed. Use SEPARATOR_CHAR to distinguish instructions");
	while (all_commands[(int)command_size])
		command_size++;
	if (command_size != instructions.instruct_arg[i][0])
	{
		ft_printf("[[red]][[bold]][[swapp]]ERROR\n[[end]]");
		ft_printf("Expected %d arguments ; got % d\n", instructions.instruct_arg[i][0], command_size);
		ft_myexit(ft_strjoin("incorrect argument number passed to instruction : ", instructions.names[i]));
	}
	while (j < command_size)
	{
		ft_printf("for command number %d and name : %s and argument number : %d\n", (int)i, instructions.names[i], j);
		check_1_command(all_commands[(int)j], instructions.instruct_arg[i][(int)j + 1]);
		j++;
	}
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

char	**split_instructions(char *line, t_instruction instruction)
{
	char	**line_split;
	int		instruction_passed;
	int		i;
	int		j;

	i = 0;
	line_split = NULL;
	instruction_passed = 0;
	while (line[i])
	{
		j = -1;
		while (instruction_passed && i > 0 && line[i + 1] && line[i] == ' ') 
			line = delete_redudant_char(line, i);
		while (!instruction_passed && j < INSTRUCT_NBR)
		{
			if (!(ft_memcmp(&line[i], instruction.names[j], ft_strlen(instruction.names[j]))))
			{
				if (line[i + ft_strlen(instruction.names[j]) + 1])
					i += ft_strlen(instruction.names[j]) + 1;
				else
					ft_myexit("incomplete instruction");
				instruction_passed = 1;
			}
			j++;
		}
		i++;
	}
	if (!(line_split = ft_split_whitespaces(line)))
		ft_myexit("error in ft_split");
	return (line_split);
}

int		cut_comment(char **line)
{
	int i;
	int	fill_line;

	i = 0;
	fill_line = 0;
	while ((*line)[i])
	{
		if ((*line)[i] == '#')
		{
			(*line)[i] = 0;
			break ;
		}
		else if ((*line)[i] > 33)
			fill_line = 1;
		i++;
	}
	return (fill_line);
}

void	check_instructions(int fd_write, char *line, t_instruction instruction)
{
	char	**line_split;
	int		i;

	i = 0;
	if (!(cut_comment(&line)))
		return ;
	line_split = split_instructions(line, instruction);
	ft_printf("[[blue]][[underline]]Checking : %s\n[[end]]", line);
	while (ft_strcmp(line_split[0], instruction.names[i]) && i < INSTRUCT_NBR)
		i++;
	if (i == INSTRUCT_NBR)
	{
		// You should here check if what you got is a label followed by instructions
		ft_myexit(ft_strjoin("bad instruction name : ", line_split[0]));
	}
	check_instruction_arguments(line_split[1], i, instruction);
	(void)fd_write;
//	write_instruction(instruction, fd_write, i);
	ft_memdel((void**)&line);
	ft_tabdel((void***)&line_split);
}

void	read_instructions(int fd_read, int fd_write)
{
	int				ret;
	char			*buf;
	t_instruction	instructions;

	buf = NULL;
	instructions = set_instructions();
	while ((ret = get_next_line(fd_read, &buf, '\n') > 0))
	{
		if (ft_strlen(buf) > 0)
			check_instructions(fd_write, buf, instructions);
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
	ft_memcpy(&name[strlen - 1], "cor", 3);
	fd_read = open(av[1], O_RDONLY);
	fd_write = open(name, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
	if (fd_read <= 0 || fd_write <= 0)
		ft_myexit("Open error");
	// you should check instructions and name and comment before inputing
	input_magic(fd_write);
	output_name_comment(fd_read, fd_write, PROG_NAME_LENGTH);
	read_instructions(fd_read, fd_write);
}
