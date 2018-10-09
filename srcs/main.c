/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/16 17:37:23 by lazrossi          #+#    #+#             */
/*   Updated: 2018/10/09 22:57:24 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/includes/libft.h"
#include "../includes/asm.h"
#include <unistd.h>
#include <stdlib.h>

#include <fcntl.h>
void	write_file(t_fd fd, t_info *info)
{
	if (ft_check_little_endianness())
	info->header.prog_size = little_endian_to_big(info->write_pos - sizeof(info->header), sizeof(int));
	fd.write = open(info->file_name, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
	if (fd.write == -1)
		ft_myexit("open error in write_file");
	ft_memcpy(info->to_write, &info->header, sizeof(info->header));
	write(fd.write, info->to_write, info->write_pos);
	close(fd.write);
}

void	malloc_resize_write_size(t_info *info)
{
	char	*new_buffer;

	new_buffer = NULL;
	if (info->to_write)
		info->to_write_size *= 2;
	if (!(new_buffer = malloc(sizeof(char) * info->to_write_size)))
		ft_myexit("malloc error in malloc_resize_write_size");
	ft_memset(new_buffer, 0, info->to_write_size);
	if (info->to_write_size != sizeof(info->header))
	{
		ft_memcpy(new_buffer, info->to_write, info->to_write_size / 2);
		ft_memdel((void**)&(info->to_write));
	}
	info->to_write = new_buffer;
}

static int		g_initial_parser[8][8][2] = {
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


static unsigned int    g_alpha[128] = {
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




void	malloc_resize_token_tab(t_info *info)
{
	t_token	*new_tab;
	unsigned int		i;

	new_tab = NULL;
	if (info->tokens)
		info->token_struct_size *= 2;
	if (!(new_tab = malloc(sizeof(t_token) * info->token_struct_size)))
		ft_myexit("malloc error in malloc_resize_token_tab");
	if (info->tokens)
	{
		i = 0;
		while (i < info->token_nbr)
		{
			new_tab[i] = info->tokens[i];
			i++;
		}
		ft_memdel((void**)&(info->tokens));
	}
	info->tokens = new_tab;
}

/*
typedef struct	s_funct
{
	int			begin_state;
	int			end_state;
	void		(*func_pointer)(t_info*, int);
}				t_funct;

static t_funct	func_tab[5]  = {
	{ FUNCTION, ARG_INPUT_WAIT, new_function_or_label
	};
	*/

void	save_tokens(t_info *info, int *command_start, int begin_state, int end_state)
{
	static int debug = 1;
	if (DEBUG)
		token_debug(info, command_start, begin_state, end_state);
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
		end_state = g_initial_parser
			[begin_state][g_alpha[(int)info->file[info->read_pos]]][0];
		if (begin_state != end_state)
			save_tokens(info, &command_start, begin_state, end_state);
		info->read_pos += g_initial_parser
			[begin_state][g_alpha[(int)info->file[info->read_pos]]][1];
		if (end_state == 1)
			command_start = info->read_pos;
	}
	if (DEBUG)
		print_function_list(info);
}

// check at the beginning if integers are of size 4 (always the case)
int		main(int ac, char **av)
{
	t_fd			fd;
	t_info			info;

	fd.read = -1;
	fd.write = -1;
	info.label_info.label_list = NULL;
	info.label_info.label_categories = LABEL_INITIAL_NBR;
	info.label_info.n = 0;
	info.error = 0;

	info.to_write = NULL;
	info.to_write_size = sizeof(info.header);
	info.write_pos = sizeof(info.header);

	malloc_resize_write_size(&info);
	if (ac != 2)
		ft_myexit("You need to pass not more or less than one file to assemble");

	set_name_open_fd(&info, &fd, av[1]);
	read_file(&info, fd);
	info.read_pos = 0;

	store_name_comment(&info, PROG_NAME_LENGTH);
	lexe_tokens(&info);
	parse_functions(&info);
	   check_label_list(&(info.label_info));
	   input_labels(&(info.label_info), &info);
	if (0 == info.error)
	 	write_file(fd, &info);
	close(fd.read);
	//	sleep(45);
}
