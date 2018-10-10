/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_binary.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/20 10:48:41 by lazrossi          #+#    #+#             */
/*   Updated: 2018/10/11 01:01:45 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/includes/libft.h"
#include "../includes/asm.h"

static void	input_magic(t_info *info)
{
	info->header.magic = COREWAR_EXEC_MAGIC;
	if (ft_check_little_endianness())
		info->header.magic =
			little_endian_to_big(
					COREWAR_EXEC_MAGIC, sizeof(info->header.magic));
	else
		info->header.magic = COREWAR_EXEC_MAGIC;
}

static void	store_name_comment_2(t_info *info, int which, int quotes_end)
{
	if (!(ft_is_white_space_line(&info->file[info->read_pos] + 2 + quotes_end)))
		ft_myexit("Dangling characters\
after quotes in line defining name or comment.");
	if (quotes_end > which)
		ft_myexit("Your champion's name or your comment is too long");
	if (which == PROG_NAME_LENGTH)
		ft_memset(&(info->header), 0, sizeof(info->header));
	ft_memcpy(which == (PROG_NAME_LENGTH) ?
	info->header.prog_name :
	info->header.comment, &info->file[info->read_pos] + 1, quotes_end);
	info->read_pos += quotes_end + 2;
	if (which == PROG_NAME_LENGTH)
	{
		input_magic(info);
		store_name_comment(info, COMMENT_LENGTH);
	}
}

void		store_name_comment(t_info *info, int which)
{
	int		quotes_end;

	if (!(ft_strchr(&info->file[(int)info->read_pos], '.')))
		ft_myexit("File must begin by a . for name and comment lines");
	info->read_pos = (unsigned int)
				(ft_strchr(&info->file[(int)info->read_pos], '.') - info->file);
	if (ft_memcmp(&info->file[info->read_pos], which == PROG_NAME_LENGTH ?
		".name" : ".comment",
		ft_strlen(which == PROG_NAME_LENGTH ? ".name" : ".comment")))
		ft_myexit("Your name or comment line must start by .name or .comment");
	if (NULL == ft_strnchr(&info->file[info->read_pos], '"',
	ft_strchr(&info->file[info->read_pos], '\n') - &info->file[info->read_pos]))
		ft_myexit("No quotes after .name or .comment");
	info->read_pos = ft_strnchr(&info->file[info->read_pos], '"',
	ft_strchr(&info->file[info->read_pos], '\n') - &info->file[info->read_pos])
		- info->file;
	quotes_end = 0;
	while (info->file[info->read_pos + quotes_end] &&
			info->file[info->read_pos + quotes_end + 1] != '"')
		quotes_end++;
	if (!(info->file[info->read_pos + quotes_end]))
		ft_myexit("Dangling quotes after .name or .comment");
	store_name_comment_2(info, which, quotes_end);
}
