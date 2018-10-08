/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_argument_labels.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/08 12:35:09 by lazrossi          #+#    #+#             */
/*   Updated: 2018/10/08 12:35:34 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/asm.h"
#include "../libft/includes/libft.h"

void	new_argument(t_info *info, int command_start)
{
	int i;

	i = 1;
	while (info->tokens[info->token_nbr - 1].token[i][0] && i < 4)
		i++;
	if (i == 4)
	{
		info->error = 1;
		// go into error mode
	}
	else
	{
		info->tokens[info->token_nbr - 1].token[i][0] = command_start;
		info->tokens[info->token_nbr - 1].token[i][1] = info->read_pos;
	}
}

void	new_function_or_label(t_info *info, int command_start, int func_or_label)
{
	t_token new;

	new.token[0][0] = command_start;
	new.token[0][1] = info->read_pos;
	new.token[1][0] = 0;
	new.token[2][0] = 0;
	new.token[3][0] = 0;
	new.func_nbr = func_or_label;
	// you need to save line number here
	info->tokens[info->token_nbr] = new;
	info->token_nbr++;
	if (info->token_nbr == info->token_struct_size)
		malloc_resize_token_tab(info);
}
