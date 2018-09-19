/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/16 21:05:44 by lazrossi          #+#    #+#             */
/*   Updated: 2018/09/19 08:17:37 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASM_H
# define ASM_H

# define PROG_NAME_LENGTH		(128)
# define COMMENT_LENGTH			(2048)

#define IND_SIZE				2
#define REG_SIZE				4
#define DIR_SIZE				REG_SIZE

# define COREWAR_EXEC_MAGIC		0xea83f3
#define COMMENT_CHAR			'#'
#define NAME_CMD_STRING			".name"
#define COMMENT_CMD_STRING		".comment"
#define INSTRUCT_NBR			16
#define T_DIR					4
#define T_IND 					2
#define T_REG 					1

#define COMMENT_CHAR			'#'
#define LABEL_CHAR				':'
#define DIRECT_CHAR				'%'
#define SEPARATOR_CHAR			','
#define REG_NUMBER				16

typedef struct		s_header
{
  unsigned int		magic;
  char				prog_name[PROG_NAME_LENGTH + 1];
  unsigned int		prog_size;
  char				comment[COMMENT_LENGTH + 1];
}					t_header;

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

#endif
