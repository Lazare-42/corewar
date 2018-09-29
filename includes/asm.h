/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/16 21:05:44 by lazrossi          #+#    #+#             */
/*   Updated: 2018/09/29 19:30:02 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASM_H
# define ASM_H

#include "./op.h"

#define INSTRUCT_NBR			16
#define LABEL_INITIAL_NBR		1
#define INITIAL_READ_LINE_SZ	128
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

typedef struct		s_line
{
	char			*line;
	int				nbr;
	int				tokens[4][3];
}					t_line;

typedef struct		s_info
{
	t_line			*line_tokens;
	int				line_nbr;
	int				line_info_size;

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
void		write_instruction(t_info *info, unsigned char command_binary, char **all_commands, int instruction_nbr);
void		write_instruction_info(t_info *info, unsigned char command_binary, int instruction_nbr);
void		malloc_resize_write_size(t_info *info);
void		write_file(t_fd fd, t_info *info);
#endif
