/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   label_management_two.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/11 00:10:59 by lazrossi          #+#    #+#             */
/*   Updated: 2018/10/11 00:58:31 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/asm.h"
#include "../libft/includes/libft.h"

t_label	new_label_first(int pos_in_file, int len)
{
	t_label new;

	new.pos_in_file = pos_in_file;
	new.name_len = len;
	return (new);
}

t_label	new_label(t_label new, int label_pos,
		int write_pos, int byte_size)
{
	new.byte_size = byte_size;
	new.label_pos = label_pos;
	new.write_pos = write_pos;
	new.next = NULL;
	return (new);
}

void	add_new_label_type(t_label_info *info, t_label new, int i)
{
	info->n++;
	if (info->n == info->label_categories)
	{
		info->label_categories *= 2;
		malloc_resize_label_list(info);
	}
	(info->label_list)[i] = new;
}
