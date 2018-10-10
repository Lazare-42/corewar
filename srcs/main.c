/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/16 17:37:23 by lazrossi          #+#    #+#             */
/*   Updated: 2018/10/09 23:46:36 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/includes/libft.h"
#include "../includes/asm.h"
#include <unistd.h>

static void	struct_init(t_fd *fd, t_info *info)
{
	fd->read = -1;
	fd->write = -1;
	info->label_info.label_list = NULL;
	info->label_info.label_categories = LABEL_INITIAL_NBR;
	info->read_pos = 0;
	info->label_info.n = 0;
	info->error = 0;
	info->to_write = NULL;
	info->to_write_size = sizeof(info->header);
	info->write_pos = sizeof(info->header);
}

// check at the beginning if integers are of size 4 (always the case)
int		main(int ac, char **av)
{
	t_fd			fd;
	t_info			info;

	struct_init(&fd, &info);
	malloc_resize_write_size(&info);
	if (ac != 2)
		ft_myexit("You need to pass not more or less than one file to assemble");
	set_name_open_fd(&info, &fd, av[1]);
	read_file(&info, fd);
	store_name_comment(&info, PROG_NAME_LENGTH);
	lexe_tokens(&info);
	parse_functions(&info);
	check_label_list(&(info.label_info));
	input_labels(&(info.label_info), &info);
	if (0 == info.error)
		write_file(fd, &info);
	close(fd.read);
}
