#include "../includes/asm.h"
#include "../libft/includes/libft.h"
#include <fcntl.h>
#include <stdlib.h>

void	set_name_open_fd(t_info *info, t_fd *fd, char *to_open)
{
	printf("->%s\n", to_open);
	ft_memset(info->file_name, 0, 256);
	ft_memcpy(info->file_name, "./", 2);
	ft_memcpy(info->file_name + 2, to_open, ft_strlen(to_open));
	ft_memcpy(info->file_name + ft_strlen(to_open) - 1 + 2, "cor", 3);
	printf("=>%s\n", info->file_name);
	fd->read = open(to_open, O_RDONLY);
	if (fd->read <= 0)
		ft_myexit("Open error");
}

static char	**file_lines(int nbr, char	**file_lines)
{
	char	**new;
	int		i;

	new = NULL;
	i = 0;
	if (!(new = malloc(sizeof(char *) * nbr)))
		ft_myexit("malloc error in file_lines");
	while (i < nbr)
	{
		new[i] = NULL;
		i++;
	}
	if (file_lines)
	{
		i = 0;
		while (i < nbr / 2)
		{
			new[i] = file_lines[i];
			i++;
		}
		ft_tabdel((void***)&file_lines);
	}
	return (file_lines = new);
}

void	read_file(t_info *info, t_fd fd)
{
	int		gnl_ret;
	char	*buf;
	int		i;

	i = 0;
	info->file_read = NULL;
	info->file_lines_nbr = FILE_INITIAL_SIZE;
	info->file_read = file_lines(FILE_INITIAL_SIZE, info->file_read);
	while ((gnl_ret = get_next_line(fd.read, &buf, '\n')) > 0)
	{
		if (i >= info->file_lines_nbr)
		{
			info->file_lines_nbr *= 2;
			info->file_read = file_lines(info->file_lines_nbr, info->file_read);
		}
		(info->file_read)[i] = buf;
		i++;
	}
	if (gnl_ret < 0)
		ft_myexit("get_next_line error");
}

//	fd.write = open(info.file_name, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);

