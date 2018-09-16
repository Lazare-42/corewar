#ifndef ASM_H
# define ASM_H

# define PROG_NAME_LENGTH		(128)
# define COMMENT_LENGTH			(2048)
# define COREWAR_EXEC_MAGIC		0xea83f3
#define COMMENT_CHAR			'#'
#define NAME_CMD_STRING			".name"
#define COMMENT_CMD_STRING		".comment"

typedef struct		header_s
{
  unsigned int		magic;
  char				prog_name[PROG_NAME_LENGTH + 1];
  unsigned int		prog_size;
  char				comment[COMMENT_LENGTH + 1];
}					header_t;

#endif
