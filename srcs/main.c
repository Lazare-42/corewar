/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/16 17:37:23 by lazrossi          #+#    #+#             */
/*   Updated: 2018/09/17 13:27:08 by lazrossi         ###   ########.fr       */
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


t_instruction	set_instructions_argument_types(t_instruction instructions)
{
	instructions.instruct_arg[1][1] = T_DIR;
	instructions.instruct_arg[2][1] = T_DIR;
	instructions.instruct_arg[2][2] = T_IND | T_REG;
	instructions.instruct_arg[3][1] = T_REG;
	instructions.instruct_arg[3][2] = T_IND | T_REG;
	instructions.instruct_arg[3][1] = T_REG;
	instructions.instruct_arg[3][2] = T_REG;
	instructions.instruct_arg[3][3] = T_REG;
	instructions.instruct_arg[4][1] = T_REG;
	instructions.instruct_arg[4][2] = T_REG;
	instructions.instruct_arg[4][3] = T_REG;
	instructions.instruct_arg[5][1] = T_REG | T_DIR | T_IND;
	instructions.instruct_arg[5][2] = T_REG | T_DIR | T_IND;
	instructions.instruct_arg[5][3] = T_REG;
	instructions.instruct_arg[6][1] = T_REG | T_DIR | T_IND;
	instructions.instruct_arg[6][2] = T_REG | T_DIR | T_IND;
	instructions.instruct_arg[6][3] = T_REG;
	instructions.instruct_arg[7][1] = T_REG | T_DIR | T_IND;
	instructions.instruct_arg[7][2] = T_REG | T_DIR | T_IND;
	instructions.instruct_arg[7][3] = T_REG;
	instructions.instruct_arg[8][1] = T_DIR;
	instructions.instruct_arg[9][1] = T_REG | T_DIR | T_DIR;
	instructions.instruct_arg[9][2] = T_DIR | T_REG;
	instructions.instruct_arg[9][3] = T_REG;
	instructions.instruct_arg[10][1] = T_REG;
	instructions.instruct_arg[10][2] = T_REG | T_DIR | T_IND;
	instructions.instruct_arg[10][3] = T_REG | T_DIR;
	instructions.instruct_arg[11][1] = T_DIR;
	instructions.instruct_arg[12][1] = T_DIR;
	instructions.instruct_arg[12][2] = T_IND | T_REG;
	instructions.instruct_arg[13][1] = T_REG | T_DIR | T_IND;
	instructions.instruct_arg[13][2] = T_REG | T_DIR;
	instructions.instruct_arg[13][3] = T_REG;
	instructions.instruct_arg[14][1] = T_DIR;
	instructions.instruct_arg[15][1] = T_REG;
	return (instructions);
}

t_instruction	set_instructions_argument_nbr(t_instruction instructions)
{
	instructions.instruct_arg[0][0] = 1;
	instructions.instruct_arg[1][0] = 2;
	instructions.instruct_arg[2][0] = 2;
	instructions.instruct_arg[3][0] = 3;
	instructions.instruct_arg[4][0] = 3;
	instructions.instruct_arg[5][0] = 3;
	instructions.instruct_arg[6][0] = 3;
	instructions.instruct_arg[7][0] = 3;
	instructions.instruct_arg[8][0] = 1;
	instructions.instruct_arg[9][0] = 3;
	instructions.instruct_arg[10][0] = 3;
	instructions.instruct_arg[11][0] = 1;
	instructions.instruct_arg[12][0] = 2;
	instructions.instruct_arg[13][0] = 3;
	instructions.instruct_arg[14][0] = 1;
	instructions.instruct_arg[15][0] = 1;
	return (set_instructions_argument_types(instructions));
}

t_instruction	set_instructions(void)
{
	t_instruction instructions;

	ft_memcpy(instructions.names[0], "live", ft_strlen("live"));
	ft_memcpy(instructions.names[1], "ld", ft_strlen("ld"));
	ft_memcpy(instructions.names[2], "st", ft_strlen("st"));
	ft_memcpy(instructions.names[3], "add", ft_strlen("add"));
	ft_memcpy(instructions.names[4], "sub", ft_strlen("sub"));
	ft_memcpy(instructions.names[5], "and", ft_strlen("and"));
	ft_memcpy(instructions.names[6], "or", ft_strlen("or"));
	ft_memcpy(instructions.names[7], "xor", ft_strlen("xor"));
	ft_memcpy(instructions.names[8], "zjmp", ft_strlen("zjmp"));
	ft_memcpy(instructions.names[9], "ldi", ft_strlen("ldi"));
	ft_memcpy(instructions.names[10], "sti", ft_strlen("sti"));
	ft_memcpy(instructions.names[11], "fork", ft_strlen("fork"));
	ft_memcpy(instructions.names[12], "lld", ft_strlen("lld"));
	ft_memcpy(instructions.names[13], "lldi", ft_strlen("lldi"));
	ft_memcpy(instructions.names[14], "lfork", ft_strlen("lfork"));
	ft_memcpy(instructions.names[15], "aff", ft_strlen("aff"));
	return (set_instructions_argument_nbr(instructions));
}

void	check_1_command(char *command, char match)
{
	if (match & T_DIR)
}

void	check_instruction_arguments(char *commands, int i, t_instruction instructions)
{
	char	**all_commands;
	char	command_size;
	char	j;

	j = 0;
	command_size = 0;
	if (!(all_commands = ft_split_char(commands, ',')))
		ft_myexit("ft_split failed. Use commas to distinguish instructions");
	while (all_commands[(int)command_size])
		command_size++;
	if (command_size != instructions.instruct_arg[i][0])
		ft_myexit(ft_strjoin("incorrect argument number passed to instruction : ", instructions.names[i]));
	while (j < command_size)
	{
		check_1_command(all_commands[(int)j], instructions.instruct_arg[i][(int)j]);
		j++;
	}
}

void	check_instructions(int fd_write, char *line, t_instruction instruction)
{
	char	**line_split;
	int		i;

	i = 0;
	line_split = NULL;
	if (!(line_split = ft_split_char(line, ' ')))
		ft_myexit("error in ft_split");
	while (ft_strcmp(line_split[0], instruction.names[i]) && i < INSTRUCT_NBR)
		i++;
	if (i == INSTRUCT_NBR)
		ft_myexit("bad instruction name");
	check_instruction_arguments(line_split[1], i, instruction);
	write_instruction(instruction, fd_write, i);
	ft_memdel((void**)&line);
	ft_tabdel((void***)&instruction);
}

void	read_instructions(int fd_read, int fd_write)
{
	int				ret;
	char			*buf;
	t_instruction	instructions;

	buf = NULL;
	instructions = set_instructions();
	while ((ret = get_next_line(fd_read, &buf, '\n') > 1))
		check_instructions(fd_write, buf, instructions);
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
	input_magic(fd_write);
	output_name_comment(fd_read, fd_write, PROG_NAME_LENGTH);
}
