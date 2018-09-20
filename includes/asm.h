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

typedef struct		s_instructions
{
	char			names[INSTRUCT_NBR][64];
	unsigned char	instruct_arg[INSTRUCT_NBR][4];
}					t_instruction;

typedef struct		s_labels
{
	char			*name;
	int				anchor;
	int				position;
	void			*next;
}					t_label;

t_instruction	set_instructions(void);
t_label			*label_list(char	*label, int is_anchor);
void		store_name_comment(int fd_read, t_header *info, int which);

#endif
