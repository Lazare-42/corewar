/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/16 21:05:44 by lazrossi          #+#    #+#             */
/*   Updated: 2018/09/28 14:58:22 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASM_H
# define ASM_H

#include "./op.h"

#define INSTRUCT_NBR			16
#define FILE_INITIAL_SIZE		256 
#define LABEL_INITIAL_NBR		1
#define INITIAL_WRITE_SIZE		4096
#define HEADER_SIZE				0x890
#define INITIAL_WRITE_POS		PROG_NAME_LENGTH + COMMENT_LENGTH
#define COMMAND_INSTRUCTIONS	2 			

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
	int				label_pos;
	int				write_pos;
	int				byte_size;
	void			*next;
}					t_label;

typedef struct		s_label_info
{
	int				n;
	int				label_categories;
	t_label			*label_list;
}					t_label_info;

typedef struct		s_info
{
	char			**file_read;
	int				file_lines_nbr;
	char			file_name[256];
	t_label_info	label_info;
	t_header		header;
	char			*to_write;
	int				cmd_size;
	int				cmd_begin_pos;
	int				to_write_size;
	int				write_pos;
}					t_info;

t_instruction	set_instructions(void);
void		label_list(t_label_info *info, t_label new);
void		store_name_comment(t_info *info, int which);
void		set_name_open_fd(t_info *info, t_fd *fd, char *to_open);
void		read_file(t_info *info, t_fd fd);
void		print_label_list(t_label_info *info);
void		check_label_list(t_label_info *info);
void		input_labels(t_label_info *label_info, t_info *info);
t_label 	new_label(char *name, int label_pos, int write_pos, int byte_size);
#endif
