/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/16 17:37:23 by lazrossi          #+#    #+#             */
/*   Updated: 2018/10/08 12:25:01 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/includes/libft.h"
#include "../includes/asm.h"
#include <unistd.h>
#include <stdlib.h>

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


static int    g_alpha[128] = {
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


int		check_label(char *label)
{
	int i;

	i = 0;
	if (!(label) || label[0] == 0)
		return (0);
	while (label[i])
	{
		if (!(ft_strchr(LABEL_CHARS, label[i])))
			return (0);
		i++;
	}
	return (1);
}

char	check_1_command(char *command, char match)
{
	if (match & T_DIR && command[0] == DIRECT_CHAR)
	{
		if (command[1] == LABEL_CHAR)
			if (check_label(&command[2]))
				return (DIR_CODE);
		if (ft_isnumber(&command[1]))
			return (DIR_CODE);
	}
	if (match & T_IND)
	{
		if (command[0] == LABEL_CHAR)
			if (check_label(&command[1]))
				return (IND_CODE);
		if (ft_isnumber(&command[0]))
			return (IND_CODE);
	}
	if (match & T_REG)
		if (command[0] == 'r')
			if (ft_isnumber(&command[1]) && ft_atoi(&command[1]) <= REG_NUMBER)
				return (REG_CODE);
	ft_myexit(ft_strjoin(command, " (command) invoked with the wrong argument")); 
	return (0);
}

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


void	save_tokens(t_info *info, int *command_start, int begin_state, int end_state)
{
	static int debug = 1;
	if (DEBUG)
		token_debug(info, command_start, begin_state, end_state);
	unsigned int tmp;

	tmp = *command_start;
	if (begin_state == ERROR || end_state == ERROR)
	{
		// save error
	}
	if (begin_state == FUNCTION && end_state == ARG_INPUT_WAIT)
	{
		// save function
		new_function_or_label(info, *command_start, FUNCTION);
	}
	if (begin_state == FUNCTION && end_state == ANCHOR_LABEL)
	{
		new_function_or_label(info, *command_start, LABEL);
	}
	if (begin_state == ARG_INPUT && (end_state == DEFAULT || end_state == ARG_INPUT_WAIT || end_state == WP_AFTER_ARG))
	{
		new_argument(info, *command_start);
		// add argument to function
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
	print_function_list(&info);
	//	ft_printf("%s\n", &info.file[info.read_pos]);
	//	ft_printf("[[red]]%d %d\n[[end]]", ',', g_alpha[44]);
	/*
	   check_label_list(&(info.label_info));
	   input_labels(&(info.label_info), &info);
	 */
	if (0 == info.error)
	 	write_file(fd, &info);
	close(fd.read);
	//	sleep(45);
}
