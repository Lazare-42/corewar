/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_endianness.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/25 17:58:43 by lazrossi          #+#    #+#             */
/*   Updated: 2018/10/11 01:05:01 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../includes/libft.h"

/*
** this function returns 1 if the machine is little-endian
*/

int						ft_check_little_endianness(void)
{
	unsigned int	x;
	char			*c;

	x = 1;
	c = NULL;
	c = (char*)&x;
	return ((int)*c);
}

unsigned long long		little_endian_to_big(unsigned long long src,
		int sizeof_var)
{
	unsigned char					tmp[sizeof(unsigned long long)];
	unsigned char					*number;
	unsigned long long				ret;
	unsigned long long				i;

	number = (void*)&src;
	i = -1;
	while (++i < sizeof(unsigned long long))
		tmp[i] = number[sizeof(unsigned long long) - i - 1];
	ft_memcpy(&ret, tmp + sizeof(unsigned long long) - sizeof_var, sizeof_var);
	return (ret);
}
