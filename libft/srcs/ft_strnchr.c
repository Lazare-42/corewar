/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/11 01:05:25 by lazrossi          #+#    #+#             */
/*   Updated: 2018/10/11 01:05:26 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_strnchr(const char *s, int c, unsigned int size)
{
	unsigned int i;

	i = 0;
	if (s[i] == c)
		return ((char*)&s[i]);
	while (i < size && s[i])
	{
		i++;
		if (s[i] == c)
			return ((char*)&s[i]);
	}
	return (NULL);
}
