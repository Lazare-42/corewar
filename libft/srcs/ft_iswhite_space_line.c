/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_iswhite_space_line.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/11 01:04:42 by lazrossi          #+#    #+#             */
/*   Updated: 2018/10/11 01:04:45 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

int		ft_is_white_space_line(char *line)
{
	unsigned int i;

	i = 0;
	while (line[i] && line[i] != '\n')
	{
		if (line[i] != 32 && line[i] != 9)
			return (0);
		i++;
	}
	return (1);
}
