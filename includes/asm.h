/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/16 21:05:44 by lazrossi          #+#    #+#             */
/*   Updated: 2018/10/09 23:47:02 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASM_H
# define ASM_H

#include "./op.h"
#include "./asm_structs.h"

#define LABEL_INITIAL_NBR		1
#define HEADER_SIZE				0x890
#define INITIAL_WRITE_POS		PROG_NAME_LENGTH + COMMENT_LENGTH
#define COMMAND_INSTRUCTIONS	2 			

#define ERROR					7
#define DEFAULT					1
#define FUNCTION				2
#define ANCHOR_LABEL			3
#define ARG_INPUT_WAIT			4
#define ARG_INPUT				5
#define WP_AFTER_ARG			6

#define INITIAL_FILE_SIZE		128
#define INITIAL_TOKEN_NBR		64
#define INITIAL_WRITE_SIZE		4096

#define LABEL					17
#define MAX_FUNC_NBR			17

#define	XOR_FUNC_NBR			9

void			label_list(t_info *info, t_label new);
void			store_name_comment(t_info *info, int which);
void			set_name_open_fd(t_info *info, t_fd *fd, char *to_open);
void			read_file(t_info *info, t_fd fd);
void			check_label_list(t_label_info *info);
void			input_labels(t_label_info *label_info, t_info *info);
t_label 		new_label(int pos_in_file, int len, int label_pos,
		int write_pos, int byte_size);
void			new_function_or_label(t_info *info,
		int command_start, int is_label);
void			new_argument(t_info *info, int command_start);
void			parse_functions(t_info *info);
void			write_one_function(unsigned char func_nbr,
		t_token function_token, t_info *info, unsigned char cmd_binary);
int				is_num(char *to_check, unsigned int size);
int				check_label(char *label, unsigned int size);
void			malloc_resize_write_size(t_info *info);
void			malloc_resize_token_tab(t_info *info);
void			malloc_resize_label_list(t_label_info *info);
t_label			*new_malloced_label(t_label to_store);
void			write_label(t_info *info, int where,
		short distance, int byte_size);
void			write_file(t_fd fd, t_info *info);

#endif
