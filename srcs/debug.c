/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/08 08:25:32 by lazrossi          #+#    #+#             */
/*   Updated: 2018/10/08 12:29:43 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/asm.h"
#include "../libft/includes/libft.h"
#include <unistd.h>

void	print_function_list(t_info *info)
{
	unsigned int i;
	unsigned int j;

	i = 0;
	while (i < info->token_nbr)
	{
		j = 1;
		if (info->tokens[i].func_nbr == LABEL)
			ft_printf("[[yellow]][[underline]][[bold]]");
		else
			ft_printf("[[cyan]][[underline]][[bold]]");
		write(1, &(info->file[info->tokens[i].token[0][0]]), info->tokens[i].token[0][1] - info->tokens[i].token[0][0]);
		while (info->tokens[i].token[j][0] && j < 4)
		{
			ft_printf("[[end]] -> ");
			ft_printf("[[cyan]] ");
			write(1, &(info->file[info->tokens[i].token[j][0]]), info->tokens[i].token[j][1] - info->tokens[i].token[j][0]);
			ft_printf("[[end]]");
			j++;
		}
		ft_printf("[[end]]\n");
		i++;
	}
}

void	token_debug(t_info *info, int *command_start, int begin_state, int end_state)
{
	unsigned int tmp;

	tmp = *command_start;
	// commands go from [command_start ; info->read_pos [
	if (begin_state == ERROR || end_state == ERROR)
	{
		ft_printf("[[red]] ");
		while (tmp < info->read_pos)
		{
			ft_putchar((int)info->file[tmp]);
			tmp++;
		}
		ft_printf(" [[end]]");
	}
	if (begin_state == FUNCTION && end_state == ARG_INPUT_WAIT)
	{
		ft_printf("\n[[blue]]");
		while (tmp < info->read_pos)
		{
			ft_printf("[[yellow]]");
			ft_putchar((int)info->file[tmp]);
			tmp++;
		}
		ft_printf(" [[end]]");
	}
	if (begin_state == FUNCTION && end_state == ANCHOR_LABEL)
	{
		ft_printf("\n[[cyan]]");
		while (tmp < info->read_pos)
		{
			ft_putchar((int)info->file[tmp]);
			tmp++;
		}
		ft_printf("[[end]]");
	}
	if (begin_state == ARG_INPUT && (end_state == DEFAULT || end_state == ARG_INPUT || end_state == ARG_INPUT_WAIT || end_state == WP_AFTER_ARG))
	{
		while (tmp < info->read_pos)
		{
			ft_printf("[[green]]");
			ft_putchar((int)info->file[tmp]);
			tmp++;
			ft_printf("[[end]]");
		}
		ft_printf(" [[end]]");
	}
}
