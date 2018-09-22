/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/16 21:05:44 by lazrossi          #+#    #+#             */
/*   Updated: 2018/09/20 21:47:07 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASM_H
# define ASM_H

#include "./op.h"

#define INSTRUCT_NBR			16
#define FILE_INITIAL_SIZE		256 

typedef struct		s_instructions
{
	char			names[INSTRUCT_NBR][64];
	unsigned char	instruct_arg[INSTRUCT_NBR][4];
}					t_instruction;

typedef	struct		s_fd
{
	int				read;
	int				write;
}					t_fd;

typedef struct		s_labels
{
	char			*name;
	int				anchor;
	int				position;
	void			*next;
}					t_label;

typedef struct		s_info
{
	char			**file_read;
	int				file_lines_nbr;
	char			file_name[256];
	t_instruction	instructions;
	t_label			**label_list;
	t_header		header;
}					t_info;

t_instruction	set_instructions(void);
t_label			*label_list(char	*label, int is_anchor);
void		store_name_comment(t_info *info, int which);

#endif
