/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_binary.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/28 16:16:15 by lazrossi          #+#    #+#             */
/*   Updated: 2018/09/28 16:22:07 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/asm.h"
#include "../libft/includes/libft.h"
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void	malloc_resize_write_size(t_info *info)
{
	char	*new_buffer;

	new_buffer = NULL;
	if (info->to_write)
		info->to_write_size *= 2;
	if (!(new_buffer = malloc(sizeof(char) * info->to_write_size)))
		ft_myexit("malloc error in malloc_resize_write_size");
	ft_memset(new_buffer, 0, info->to_write_size);
	if (info->to_write_size != sizeof(info->header))
	{
		ft_memcpy(new_buffer, info->to_write, info->to_write_size / 2);
		ft_memdel((void**)&(info->to_write));
	}
	info->to_write = new_buffer;
}

void	write_instruction(t_info *info, unsigned char command_binary, char **all_commands, int instruction_nbr)
{
	int					read_command_binary;
	int					little_endian;
	short				indirection;
	int					reg_ind;
	char				reg;

	read_command_binary = 6;
	little_endian = ft_check_little_endianness();
	while ((command_binary >> read_command_binary) & 3 && read_command_binary >= 2)
	{
		if (((command_binary >> read_command_binary) & 3) == REG_CODE)
		{
			reg = ft_atoi(&all_commands[3 - read_command_binary / 2][1]);
			ft_memcpy((void*)&info->to_write[info->write_pos], &reg, sizeof(char));
			info->write_pos += T_REG;
		}
		else if (((command_binary >> read_command_binary) & 3)== IND_CODE)
		{
			if (all_commands[3 - read_command_binary / 2][0] != LABEL_CHAR)
			{
				indirection = ft_atoi(&all_commands[3 - read_command_binary / 2][0]);
				if (little_endian)
					indirection = little_endian_to_big(indirection, sizeof(short));
				ft_memcpy((void*)&info->to_write[info->write_pos], &indirection, (T_IND / 2));
			}
			else
				label_list(&(info->label_info), new_label(&all_commands[3 - read_command_binary / 2][1], info->cmd_begin_pos, info->write_pos, T_IND / 2));
			info->write_pos += T_IND / 2;
		}
		else if (((command_binary >> read_command_binary) & 3) == DIR_CODE)
		{
			if (all_commands[3 - read_command_binary / 2][1] == LABEL_CHAR)
				label_list(&(info->label_info), new_label(&all_commands[3 - read_command_binary / 2][2], info->cmd_begin_pos, info->write_pos, (instruction_nbr < 8 || instruction_nbr == 13) ? T_DIR * 2 : T_DIR));
			else
			{
				reg_ind = ft_atoi(&all_commands[3 - read_command_binary / 2][1]);
				if (little_endian)
					reg_ind = little_endian_to_big(reg_ind, (instruction_nbr < 8 || instruction_nbr == 13) ? T_DIR * 2 : T_DIR);
				ft_memcpy((void*)&info->to_write[info->write_pos], &reg_ind, (instruction_nbr < 8 || instruction_nbr == 13) ? T_DIR * 2 : T_DIR );
			}
			info->write_pos += (instruction_nbr < 8 || instruction_nbr == 13) ? T_DIR * 2 : T_DIR;
		}
		read_command_binary -= 2;
	}
}

void	write_instruction_info(t_info *info, unsigned char command_binary, int instruction_nbr)
{
	int		cmd_size;
	int		read_command_binary;

	read_command_binary = 6;
	info->cmd_begin_pos = info->write_pos;
	cmd_size = ((char)(command_binary << 2) || instruction_nbr == 16) ? 2 : 1;
	while (read_command_binary >= 2)
	{
		if (command_binary >> read_command_binary == REG_CODE)
			cmd_size += T_REG;
		else if (command_binary >> read_command_binary == IND_CODE)
			cmd_size += T_IND;
		else if (command_binary >> read_command_binary == DIR_CODE)
			cmd_size += (instruction_nbr < 8 || instruction_nbr == 13) ? T_DIR * 2 : T_DIR;
		read_command_binary -= 2;
	}
	if ((info->write_pos + cmd_size) > info->to_write_size)
		malloc_resize_write_size(info);
	(info->to_write)[info->write_pos] = (char)instruction_nbr;
	info->write_pos += 1;
	if ((char)(command_binary << 2) || instruction_nbr == 16)
	{
		(info->to_write)[info->write_pos] = command_binary;
		info->write_pos += 1;
	}
	//info->cmd_size = cmd_size;
}

void	write_file(t_fd fd, t_info *info)
{
	if (ft_check_little_endianness())
	info->header.prog_size = little_endian_to_big(info->write_pos - sizeof(info->header), sizeof(int));
	fd.write = open(info->file_name, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
	if (fd.write == -1)
		ft_myexit("open error in write_file");
	ft_memcpy(info->to_write, &info->header, sizeof(info->header));
	write(fd.write, info->to_write, info->write_pos);
	close(fd.write);
}
