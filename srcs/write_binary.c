/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_binary.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/20 10:48:41 by lazrossi          #+#    #+#             */
/*   Updated: 2018/09/24 14:33:01 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/includes/libft.h"
#include "../includes/asm.h"
#include <stdlib.h>
#include <unistd.h>

void	input_magic(t_info *info);

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

void	store_name_comment(t_info *info, int which)
{
	char	**split;

	split = NULL;
	while ((!(ft_strstr(info->file_read[info->file_lines_nbr], ".name")) && which == PROG_NAME_LENGTH)
		|| (!(ft_strstr(info->file_read[info->file_lines_nbr], ".comment")) && which == COMMENT_LENGTH))
		info->file_lines_nbr++;
	if (!(split = (ft_split_char(info->file_read[info->file_lines_nbr], '\"'))) || !split[1])
		ft_myexit("malloc error or invalid split");
	if ((int)ft_strlen(split[1]) > which)
		ft_myexit("Your champion's name or your comment is too long");
	ft_memset(which == PROG_NAME_LENGTH ? info->header.prog_name :
	info->header.comment, 0, which == PROG_NAME_LENGTH ?
	PROG_NAME_LENGTH + 1 : COMMENT_LENGTH + 1);
	ft_memcpy(which == (PROG_NAME_LENGTH) ?
	info->header.prog_name : info->header.comment, split[1], ft_strlen(split[1]));
	ft_tabdel((void***)&split);
	if (which == PROG_NAME_LENGTH)
	{
		input_magic(info);
		store_name_comment(info, COMMENT_LENGTH); 
	}
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

void	input_magic(t_info *info)
{
	info->header.magic = COREWAR_EXEC_MAGIC;
	if (ft_check_endianness())
		info->header.magic = little_endian_to_big(COREWAR_EXEC_MAGIC);
	else
		info->header.magic = COREWAR_EXEC_MAGIC;
}
