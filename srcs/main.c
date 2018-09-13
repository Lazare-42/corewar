#include "../libft/includes/libft.h"
#include <fcntl.h>

void output_comment(int fd_read)
{
	(void)fd_read;
}

void output_name(int fd_read, int fd_write)
{
	char	*line;
	int		i;

	line = NULL;
	i = 0;
	if (get_next_line(fd_read, &line, '\n') <= 0)
		ft_myexit("You passed an empty file");
	if (ft_strlen(line) > 128)
		ft_myexit("Your champion's name is too long");
	while (line[i])
	{
		if (i % 2 == 0)
			ft_printf(" ");
		else
			ft_dprintf(fd_write, "%2s", &line[i]);
		i++;
	}
}

int main(int ac, char **av)
{
	int		fd_read;
	int		fd_write;
	int		strlen;
	char	name[256];
	char	*tmp;

	tmp = NULL;
	if (ac != 2)
		ft_myexit("You need to pass not more or less than one file to assemble");
	if (!(tmp = ft_strjoin("../", av[1])))
			ft_myexit("malloc error");
	strlen = ft_strlen(tmp);
	ft_memcpy(name, tmp, strlen);
	ft_memdel((void**)&tmp);
	ft_memcpy(&name[strlen - 1], "cor", 3);
	fd_read = open(av[1], O_RDONLY);
	fd_write = open(name, O_RDWR | O_CREAT, S_IRWXU);
	ft_printf("%s", name);
	if (fd_read <= 0 || fd_write <= 0)
		ft_myexit("Open error");
	output_name(fd_read, fd_write);
}
