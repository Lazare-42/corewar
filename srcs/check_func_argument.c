/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_func_argument.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/09 23:45:46 by lazrossi          #+#    #+#             */
/*   Updated: 2018/10/09 23:47:03 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/asm.h"
#include "../libft/includes/libft.h"

int		check_label(char *label, unsigned int size)
{
	unsigned int i;

	i = 0;
	if (!(label) || label[0] == 0)
		return (0);
	while (i < size)
	{
		if (!(ft_strchr(LABEL_CHARS, label[i])))
			return (0);
		i++;
	}
	return (1);
}

int		is_num(char *to_check, unsigned int size)
{
	unsigned int i;

	i = 0;
	if (to_check[i] == '-')
		i++;
	while (i < size)
	{
		if (to_check[i] < '0' || to_check[i] > '9')
			return (0);
		i++;
	}
	return (1);
}
