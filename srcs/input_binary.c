/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_binary.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/28 16:16:15 by lazrossi          #+#    #+#             */
/*   Updated: 2018/10/10 12:50:10 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/asm.h"
#include "../libft/includes/libft.h"

static void	write_one_reg(t_info *info, int instruct_pos)
{
	char	reg;

	reg = 0;
	reg = ft_atoi(&info->file[instruct_pos + 1]);
	ft_memcpy((void*)&info->to_write[info->write_pos], &reg, sizeof(char));
	info->write_pos += T_REG;
}

static void	write_one_indirect(t_info *info, int instruct_pos, int instruct_len)
{
	short	indirection;

	if (info->file[instruct_pos] != LABEL_CHAR)
	{
		indirection = ft_atoi(&info->file[instruct_pos]);
		if (ft_check_little_endianness())
			indirection = little_endian_to_big(indirection, sizeof(short));
		ft_memcpy((void*)&info->to_write[info->write_pos],
				&indirection,
				T_IND / 2);
	}
	else
	{
		label_list(info, new_label(instruct_pos + 1,
					instruct_len - 1, info->cmd_begin_pos,
					info->write_pos,
					T_IND / 2));
	}
	info->write_pos += T_IND / 2;
}

static void	write_one_direct(t_info *info, unsigned int instruct_pos,
		unsigned int instruct_len, unsigned char func_nbr)
{
	int		reg_ind;

	if (LABEL_CHAR == info->file[instruct_pos + 1])
		label_list(info,
				new_label(instruct_pos + 2, instruct_len - 2,
					info->cmd_begin_pos, info->write_pos,
					(func_nbr < XOR_FUNC_NBR || func_nbr == 13) ?
					T_DIR * 2 : T_DIR));
	else
	{
		reg_ind = ft_atoi(&info->file[instruct_pos + 1]);
		if (ft_check_little_endianness())
			reg_ind = little_endian_to_big(reg_ind,
					func_nbr < XOR_FUNC_NBR || func_nbr == 13 ?
					T_DIR * 2 : T_DIR);
		ft_memcpy((void*)&info->to_write[info->write_pos],
				&reg_ind,
				(func_nbr < XOR_FUNC_NBR || func_nbr == 13) ?
				T_DIR * 2 : T_DIR);
	}
	if (func_nbr < XOR_FUNC_NBR || 13 == func_nbr)
		info->write_pos += T_DIR * 2;
	else
		info->write_pos += T_DIR;
}

static void	write_function_arguments(t_info *info, t_token function_token,
		unsigned char func_nbr, unsigned char cmd_binary)
{
	unsigned int	read_command_binary;
	unsigned int	instruct_pos;
	unsigned int	instruct_len;

	read_command_binary = 6;
	while ((cmd_binary >> read_command_binary) & 3 && read_command_binary >= 2)
	{
		instruct_pos = function_token.token[1 + 3 - read_command_binary / 2][0];
		instruct_len = function_token.token[1 + 3 - read_command_binary / 2][1]
			- instruct_pos;
		if (((cmd_binary >> read_command_binary) & 3) == REG_CODE)
			write_one_reg(info, instruct_pos);
		if (((cmd_binary >> read_command_binary) & 3) == IND_CODE)
			write_one_indirect(info, instruct_pos, instruct_len);
		if (((cmd_binary >> read_command_binary) & 3) == DIR_CODE)
			write_one_direct(info, instruct_pos, instruct_len, func_nbr);
		read_command_binary -= 2;
	}
}

void		write_one_function(unsigned char func_nbr,
		t_token function_token, t_info *info, unsigned char cmd_binary)
{
	(info->to_write)[info->write_pos] = func_nbr;
	info->cmd_begin_pos = info->write_pos;
	info->write_pos += 1;
	if ((char)(cmd_binary << 2) || func_nbr == 16)
	{
		(info->to_write)[info->write_pos] = cmd_binary;
		info->write_pos += 1;
	}
	write_function_arguments(info, function_token, func_nbr, cmd_binary);
}
