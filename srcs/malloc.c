/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/10 13:05:18 by lazrossi          #+#    #+#             */
/*   Updated: 2018/10/10 13:15:18 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/asm.h"
#include "../libft/includes/libft.h"
#include <stdlib.h>

void	malloc_resize_write_size(t_info *info)
{
	char	*new_buffer;

	new_buffer = NULL;
	if (info->to_write)
		info->to_write_size *= 2;
	if (!(new_buffer = malloc(sizeof(char) * info->to_write_size)))
		ft_myexit("malloc error in malloc_resize_write_size");
	ft_memset(new_buffer, 0, info->to_write_size);
	if (info->to_write_size != sizeof(info->header))
	{
		ft_memcpy(new_buffer, info->to_write, info->to_write_size / 2);
		ft_memdel((void**)&(info->to_write));
	}
	info->to_write = new_buffer;
}

void	malloc_resize_token_tab(t_info *info)
{
	t_token				*new_tab;
	unsigned int		token_counter;

	new_tab = NULL;
	if (info->tokens)
	{
		info->token_struct_size *= 2;
	}
	if (NULL == (new_tab = malloc(sizeof(t_token) * info->token_struct_size)))
	{
		ft_myexit("malloc error in malloc_resize_token_tab");
	}
	if (info->tokens)
	{
		token_counter = 0;
		while (token_counter < info->token_nbr)
		{
			new_tab[token_counter] = info->tokens[token_counter];
			token_counter += 1;
		}
		ft_memdel((void**)&(info->tokens));
	}
	info->tokens = new_tab;
}

void	malloc_resize_label_list(t_label_info *info)
{
	unsigned int	label_counter;
	t_label			*new_label_list;

	new_label_list = NULL;
	if (NULL == (new_label_list = malloc(sizeof(t_label)
					* info->label_categories)))
	{
		ft_myexit("malloc error in malloc_resize_label_list");
	}
	if (info->label_list)
	{
		label_counter = 0;
		while (label_counter < info->label_categories / 2)
		{
			new_label_list[label_counter] = info->label_list[label_counter];
			label_counter += 1;
		}
		ft_memdel((void**)&(info->label_list));
	}
	info->label_list = new_label_list;
}

t_label	*new_malloced_label(t_label to_store)
{
	t_label *new;

	if (NULL == (new = malloc(sizeof(t_label))))
	{
		ft_myexit("malloc error");
	}
	*new = to_store;
	return (new);
}
