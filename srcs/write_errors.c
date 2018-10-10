/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/11 00:26:26 by lazrossi          #+#    #+#             */
/*   Updated: 2018/10/11 00:34:21 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/asm.h"
#include "../libft/includes/libft.h"
#include <unistd.h>

void	unknown_function(t_info *info, int where, int memcmp_siz)
{
	info->error = 1;
	ft_printf("Unknown [[yellow]]function[[end]] :");
	ft_printf("[[red]][[bold]][[underline]]");
	write(1, &info->file[where], memcmp_siz);
	ft_printf("[[end]]\n");
}

void	incorrect_argument(char *cmd, int siz)
{
	ft_printf("Incorrect argument passed to function : [[red]][[underline]]");
	write(1, cmd, siz);
	ft_printf("[[end]]\n");
}

void	syntax_error(t_info *info, int command_start, int size)
{
	info->error = 1;
	ft_printf("You have the following [[yellow]] syntax error[[end]] :");
	ft_printf("[[red]][[underline]]");
	write(1, &info->file[command_start], size);
	ft_printf("[[end]]\n");
}
