/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_binary.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/20 10:48:41 by lazrossi          #+#    #+#             */
/*   Updated: 2018/09/26 17:02:39 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/includes/libft.h"
#include "../includes/asm.h"

void	input_magic(t_info *info)
{
	info->header.magic = COREWAR_EXEC_MAGIC;
	if (ft_check_little_endianness())
		info->header.magic = little_endian_to_big(COREWAR_EXEC_MAGIC, sizeof(info->header.magic));
	else
		info->header.magic = COREWAR_EXEC_MAGIC;
}

void	store_name_comment(t_info *info, int which)
{
	char	**split;

	split = NULL;
	while ((!(ft_strstr(info->file_read[info->file_lines_nbr], ".name")) && which == PROG_NAME_LENGTH)
		|| (!(ft_strstr(info->file_read[info->file_lines_nbr], ".comment")) && which == COMMENT_LENGTH))
		info->file_lines_nbr++;
	if (!(split = (ft_split_char(info->file_read[info->file_lines_nbr], '"'))) || !split[1])
		ft_myexit("malloc error or invalid split");
	if ((int)ft_strlen(split[1]) > which)
		ft_myexit("Your champion's name or your comment is too long");
	if (which == PROG_NAME_LENGTH)
		ft_memset(&(info->header), 0, sizeof(info->header));
	ft_memcpy(which == (PROG_NAME_LENGTH) ?
			info->header.prog_name : info->header.comment, split[1], ft_strlen(split[1]));
	ft_tabdel((void***)&split);
	if (which == PROG_NAME_LENGTH)
	{
		input_magic(info);
		store_name_comment(info, COMMENT_LENGTH); 
	}
}
