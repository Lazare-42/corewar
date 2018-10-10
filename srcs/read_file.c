/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/11 00:59:31 by lazrossi          #+#    #+#             */
/*   Updated: 2018/10/11 00:59:59 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/asm.h"
#include "../libft/includes/libft.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

void	set_name_open_fd(t_info *info, t_fd *fd, char *to_open)
{
	ft_memset(info->file_name, 0, 256);
	ft_memcpy(info->file_name, "./", 2);
	ft_memcpy(info->file_name + 2, to_open, ft_strlen(to_open));
	ft_memcpy(info->file_name + ft_strlen(to_open) - 1 + 2, "cor", 3);
	fd->read = open(to_open, O_RDONLY);
	if (fd->read <= 0)
		ft_myexit("Open error");
}

void	malloc_file_line(char **to_cpy, unsigned int *size)
{
	char	*new;

	new = NULL;
	if ((*to_cpy))
		*size *= 2;
	else
		*size = INITIAL_FILE_SIZE;
	if (!(new = malloc(sizeof(char) * *size)))
		ft_myexit("malloc error in malloc_file_line");
	ft_memset(new, 0, *size);
	if ((*to_cpy))
	{
		ft_memcpy(new, *to_cpy, *size / 2);
		ft_memdel((void**)to_cpy);
	}
	*to_cpy = new;
}

void	read_file(t_info *info, t_fd fd)
{
	char				buf;
	unsigned char		comment_mode;
	unsigned int		read_size;
	unsigned int		malloc_size;
	int					read_ret;

	info->file = NULL;
	read_size = 0;
	comment_mode = 0;
	while ((read_ret = read(fd.read, &buf, 1)) > 0)
	{
		if (read_size == malloc_size || !(info->file))
			malloc_file_line(&(info->file), &malloc_size);
		if (buf == '#' || buf == ';')
			comment_mode = 1;
		if (comment_mode && buf == '\n')
			comment_mode = 0;
		if (0 == comment_mode)
		{
			info->file[read_size] = buf;
			read_size++;
		}
	}
	if (read_ret < 0)
		ft_myexit("read error in function read_file");
}
