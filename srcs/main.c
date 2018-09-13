#include "../libft/includes/libft.h"
#include <fcntl.h>
#include <stdlib.h>

void output_comment(int fd_read)
{
	(void)fd_read;
}

int	byte_code(char *to_convert, char **output)
{
	int		i;
	int		j;
	char	base_output[36] = "0123456789abcdefghijklmnopqrstuvwxyz";

	i = 0;
	i = 48 - ft_strlen(to_convert) % 48 + ft_strlen(to_convert) * 3; 
	if (!((*output) = malloc(sizeof(char) * i)))
		ft_myexit("malloc error in byte_code");
	ft_memset((*output), ' ', i);
	i = 0;
	j = 0;
	while (to_convert[j])
	{
		(*output)[i] = base_output[to_convert[j] % 16];
		if (i % 3 == 0 && i % 48)
			i++;
		if (i % 24 == 0 && i % 48)
			i++;
		else if (i % 48 == 0)
		{
			(*output)[i] = '\n';
			i++;
		}
		j++;
		i++;
	}
	return (i);
}

void output_name(int fd_read, int fd_write)
{
	char	*to_convert;
	char	*result;
	int		i;

	to_convert = NULL;
	i = 7;
	result = NULL;
	if (get_next_line(fd_read, &to_convert, '\n') <= 0)
		ft_myexit("You passed an empty file");
	if (ft_strlen(to_convert) < 8)
		ft_myexit("Your first to_convert has an invalid format");
	if (ft_strlen(to_convert) > 128)
		ft_myexit("Your champion's name is too long");
	byte_code(to_convert, &to_convert);
	ft_dprintf(fd_write, "%s", to_convert);
}

int main(int ac, char **av)
{
	int		fd_read;
	int		fd_write;
	int		strlen;
	char	name[256];

	if (ac != 2)
		ft_myexit("You need to pass not more or less than one file to assemble");
	strlen = ft_strlen(av[1]);
	ft_memcpy(name, av[1], strlen);
	ft_memcpy(&name[strlen - 1], "cor", 3);
	fd_read = open(av[1], O_RDONLY);
	fd_write = open(name, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
	if (fd_read <= 0 || fd_write <= 0)
		ft_myexit("Open error");
	output_name(fd_read, fd_write);
}
