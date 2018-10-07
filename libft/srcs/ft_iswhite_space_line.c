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
