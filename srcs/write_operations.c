#include "../includes/asm.h"
#include "../libft/includes/libft.h"
#include <unistd.h>
#include <fcntl.h>

void	write_file(t_fd fd, t_info *info)
{
	if (ft_check_little_endianness())
	{
		info->header.prog_size =
		little_endian_to_big
		(info->write_pos - sizeof(info->header), sizeof(int));
	}
	fd.write = open(info->file_name, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
	if (fd.write == -1)
		ft_myexit("open error in write_file");
	ft_memcpy(info->to_write, &info->header, sizeof(info->header));
	write(fd.write, info->to_write, info->write_pos);
	close(fd.write);
}

void	write_label(t_info *info, int where, short distance, int byte_size)
{
	int	distance_int;

	if (byte_size == sizeof(short))
	{
		if (ft_check_little_endianness())
			distance = little_endian_to_big(distance, sizeof(short));
		ft_memcpy((void*)&info->to_write[where], &distance, sizeof(short));
		return ;
	}
	else if (byte_size == sizeof(int))
	{
		distance_int = distance;
		if (ft_check_little_endianness())
			distance_int = little_endian_to_big(distance_int, sizeof(int));
		ft_memcpy((void*)&info->to_write[where], &distance_int, sizeof(int));
	}
}
