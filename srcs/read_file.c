#include "../includes/asm.h"
#include "../libft/includes/libft.h"
#include <fcntl.h>
#include <stdlib.h>

void	set_name_open_fd(t_info *info, t_fd *fd, char *to_open)
{
	ft_memset(info->file_name, 0, 256);
	ft_memcpy(info->file_name, "./", 2);
	ft_memcpy(info->file_name + 2, to_open, ft_strlen(to_open));
	ft_memcpy(info->file_name + ft_strlen(to_open) - 1 + 2, "cor", 3);
	fd->read = open(to_open, O_RDONLY);
	if (fd->read <= 0)
		ft_myexit("Open error");
}

void	cut_comment(char **line)
{
	int i;

	i = 0;
	while ((*line)[i])
	{
		if ((*line)[i] == '#')
		{
			(*line)[i] = 0;
			break ;
		}
		i++;
	}
}

void malloc_resize_line_tokens(t_info *info)
{
	t_line *new_line_tokens;
	int		i;

	new_line_tokens = NULL;
	if (info->line_tokens)
		info->line_info_size *= 2;
	if (!(new_line_tokens = malloc(sizeof(t_line) * info->line_info_size)))
		ft_myexit("error in malloc_resize_line_tokens");
	i = 0;
	if (info->line_tokens)
	{
		while (i < info->line_info_size / 2)
		{
			new_line_tokens[i] = info->line_tokens[i];
			i++;
		}
		ft_memdel((void**)&(info->line_tokens));
	}
	while (i < info->line_info_size)
	{
		new_line_tokens[i].line = NULL;
		i++;
	}
	info->line_tokens = new_line_tokens;
}

void	read_file(t_info *info, t_fd fd)
{
	int		gnl_ret;
	char	*buf;
	int		i;

	i = 0;
	info->line_info_size = INITIAL_READ_LINE_SZ;
	info->line_nbr = 0;
	info->line_tokens = NULL;
	malloc_resize_line_tokens(info);
	while ((gnl_ret = get_next_line(fd.read, &buf, '\n')) > 0)
	{
		if (i == info->line_info_size)
			malloc_resize_line_tokens(info);
		cut_comment(&buf);
		info->line_tokens[i].line = buf;
		info->line_tokens[i].nbr = i;
		i++;
	}
	if (gnl_ret < 0)
		ft_myexit("get_next_line error");
}
