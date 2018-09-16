/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spatriar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/11 07:33:19 by spatriar          #+#    #+#             */
/*   Updated: 2018/09/14 01:15:15 by spatriar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	ft_putchar(char c);

void	tr_base(int nbr, char *base, int l)
{
	char	result;
	int		b;
	int		c;
	int		remain;
	int		pow;

	b = 1;
	pow = 0;
	if (nbr > l - 1)
	{
		while (b < nbr)
			{
				b = b * l;
				pow++;
			}
		b = b / l;
	}
	c = nbr / b;
	result = base[c];
	ft_putchar(result);
	remain = nbr - (b * c);
	if (pow > 0)
		tr_base(remain, base, l);
}

int		check_base(char *base)
{
	int i;
	int j;
	int k;

	i = 0;
	j = 0;
	k = 0;
	while (base[i])
	{
		if (base[i] == '+' || base[i] == '-' || base[i] == 127 || base[i] < 33)
			return (0);
		i++;
	}
	while (j++ < i)
	{
		while (k++ < i)
		{
			if (j == k)
				k++;
			if (k == i)
				break;
			if (base[j] == base[k])
				return (0);
		}
		k = 0;
	}
	return (i);
}

void	ft_putnbr_base(int nbr, char *base)
{
	int		l;

	l = check_base(base);
	if (l < 2)
		return ;
	if (nbr < 0)
	{
		ft_putchar('-');
		nbr *= -1;
	}
		tr_base(nbr, base, l);
}
