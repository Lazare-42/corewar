/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/16 21:05:44 by lazrossi          #+#    #+#             */
/*   Updated: 2018/09/16 22:30:54 by lazrossi         ###   ########.fr       */
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

typedef struct		header_s
{
  unsigned int		magic;
  char				prog_name[PROG_NAME_LENGTH + 1];
  unsigned int		prog_size;
  char				comment[COMMENT_LENGTH + 1];
}					header_t;

typedef struct		instructions_s
{
	char			instructions[INSTRUCT_NBR][64];
	int				instruct_arg[INSTRUCT_NBR][1];
}					instruction_t;

#endif
