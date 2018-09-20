/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_binary.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/20 10:48:41 by lazrossi          #+#    #+#             */
/*   Updated: 2018/09/20 18:45:02 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/asm.h"
#include "../libft/includes/libft.h"
#include <stdlib.h>
#include <unistd.h>

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

	buf = NULL;
	split = NULL;
	if (get_next_line(fd_read, &buf, '\n') <= 0)
		ft_myexit("You passed an empty or incomplete file");
	while ((!(ft_strstr(buf, ".name")) && which == PROG_NAME_LENGTH) || (!(ft_strstr(buf, ".comment")) && which == COMMENT_LENGTH))
	{
		ft_memdel((void**)&buf);
		if (get_next_line(fd_read, &buf, '\n') <= 0)
			ft_myexit("You passed an empty or incomplete file");
	}
	if (!(split = (ft_split_char(buf, '\"'))) || !split[1])
		ft_myexit("malloc error or invalid split");
	if ((ft_strlen(split[1]) > PROG_NAME_LENGTH && which == PROG_NAME_LENGTH) || (ft_strlen(split[1]) > COMMENT_LENGTH && which == COMMENT_LENGTH))
		ft_myexit("Your champion's name or your comment is too long");
	ft_memdel((void**)&buf);
	(void)name;
	(void)fd_write;
//	ft_memset(name, 0, which);
	//ft_memcpy(name, split[1], ft_strlen(split[1]));
//	write(fd_write, name, which);
//	write(fd_write, "\0\0\0\0\0\0\0\0", 8);
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
