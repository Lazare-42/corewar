/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isnumber.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/28 17:03:34 by lazrossi          #+#    #+#             */
/*   Updated: 2018/09/28 17:07:10 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

int	ft_isnumber(char *to_check)
{
	int i;

	i = 0;
	if (!to_check)
		return (0);
	if (to_check[i] == '-')
		i++;
	if (!(to_check[i]))
		return (0);
	while (to_check[i])
	{
		if (!(ft_isdigit(to_check[i])))
			return (0);
		i++;
	}
	return (1);
}
