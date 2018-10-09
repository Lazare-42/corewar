#include "../libft/includes/libft.h"
#include "../includes/asm.h"

static int		g_lexer[8][8][2] = {
								//	 0		 1		 2		 3		 4		 5		 6		 7 
								//   0		 ?		 AN		 WP		 ,		 :		 %		 \n

	/* 0. End State				*/	{{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}, 

	/* 1. Default				*/	{{0, 0}, {7, 0}, {2, 0}, {1, 1}, {7, 0}, {7, 0}, {7, 0}, {1, 1}}, 

	/* 2. Function				*/	{{7, 0}, {7, 0}, {2, 1}, {4, 0}, {7, 0}, {3, 1}, {7, 0}, {7, 0}}, 

	/* 3. Label					*/	{{1, 0}, {7, 0}, {7, 0}, {1, 0}, {7, 0}, {7, 0}, {7, 0}, {1, 0}}, 

	/* 4. Argument Input wait	*/	{{1, 0}, {7, 0}, {5, 0}, {4, 1}, {7, 0}, {5, 0}, {5, 0}, {1, 0}}, 

	/* 5. Argument Input		*/	{{1, 0}, {7, 0}, {5, 1}, {6, 0}, {4, 1}, {5, 1}, {5, 1}, {1, 0}}, 

	/* 6. WP after Arg			*/	{{1, 0}, {7, 0}, {7, 0}, {6, 1}, {4, 1}, {7, 0}, {7, 0}, {1, 0}}, 

	/* 7. Error					*/	{{1, 0}, {7, 1}, {7, 1}, {1, 0}, {7, 1}, {7, 1}, {7, 1}, {1, 0}}, 

};

static unsigned int    g_ascii_tab[128] = {
	0,
	1, 1, 1, 1, 1, 1, 1, 1, 3, 7,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 3, 1, 1, 1, 1, 6, 1, 1, 1,
	1, 1, 1, 4, 2, 1, 1, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 5, 1, 1,
	1, 1, 1, 1, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	1, 1, 1, 1, 2, 1, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 1, 1, 1, 1, 1
};

static void	save_tokens(t_info *info, int *command_start, int begin_state, int end_state)
{
	static int debug = 1;
	unsigned int tmp;

	tmp = *command_start;
	if (begin_state == ERROR || end_state == ERROR)
	{
		// info->error  = 1;
		// save error
	}
	if (begin_state == FUNCTION && end_state == ARG_INPUT_WAIT)
	{
		new_function_or_label(info, *command_start, FUNCTION);
	}
	if (begin_state == FUNCTION && end_state == ANCHOR_LABEL)
	{
		new_function_or_label(info, *command_start, LABEL);
	}
	if (begin_state == ARG_INPUT && (end_state == DEFAULT || end_state == ARG_INPUT_WAIT || end_state == WP_AFTER_ARG))
	{
		new_argument(info, *command_start);
	}
	*command_start = info->read_pos;
	debug++;
}

void	lexe_tokens(t_info *info)
{
	int begin_state;
	int end_state;
	int command_start;

	end_state = 1;
	begin_state = 1;
	command_start = info->read_pos;
	info->tokens = NULL;
	info->token_struct_size = INITIAL_TOKEN_NBR;
	info->token_nbr = 0;
	malloc_resize_token_tab(info);
	while (begin_state)
	{
		begin_state = end_state;
		end_state = g_lexer
			[begin_state][g_ascii_tab[(int)info->file[info->read_pos]]][0];
		if (begin_state != end_state)
			save_tokens(info, &command_start, begin_state, end_state);
		info->read_pos += g_lexer
			[begin_state][g_ascii_tab[(int)info->file[info->read_pos]]][1];
		if (end_state == 1)
			command_start = info->read_pos;
	}
}
