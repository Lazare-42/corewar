/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazrossi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/16 17:37:23 by lazrossi          #+#    #+#             */
/*   Updated: 2018/09/30 14:17:53 by lazrossi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/includes/libft.h"
#include "../includes/asm.h"
#include <unistd.h>

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

static int		g_initial_parser[8][8][2] = {
							//	 0		 1		 2		 3		 4		 5		 6		 7 
							//   0		 ?		 AN		 WP		 ,		 :		 %		 \n
	
/* 0. End State				*/	{{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}, 
                                                                                               
/* 1. Default				*/	{{0, 0}, {7, 0}, {2, 0}, {1, 1}, {7, 0}, {7, 0}, {7, 0}, {1, 1}}, 
                                                                                               
/* 2. Function				*/	{{7, 0}, {7, 0}, {2, 1}, {4, 0}, {7, 0}, {3, 1}, {7, 0}, {7, 0}}, 
                                                                                               
/* 3. Label					*/	{{1, 0}, {7, 0}, {7, 0}, {1, 0}, {7, 0}, {7, 0}, {7, 0}, {1, 0}}, 
                                                                                               
/* 4. Argument Input wait	*/	{{1, 0}, {7, 0}, {5, 0}, {4, 1}, {7, 0}, {7, 0}, {5, 0}, {1, 0}}, 
                                                                                               
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
	1, 1, 1, 1, 1, 1, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 1, 1, 1, 1, 1
};

void	save_tokens(char *line, int *command_start, int i, int begin_state, int end_state)
{
	int tmp;

	tmp = *command_start;
	while (tmp < i)
	{
		ft_printf("[[yellow]]");
		ft_putchar((int)line[tmp]);
		tmp++;
		ft_printf("[[end]]");
	}
	if (begin_state == 7)
		ft_printf("[[red]]error%d", begin_state);
	if (end_state == 7)
		ft_printf("[[red]] error %d[[end]]", end_state);
	if (begin_state == 2 && end_state == 4)
	{
		*command_start = i;
		ft_printf("[[green]] command[[end]]\n");
	}
	if (begin_state == 2 && end_state == 3)
	{
		*command_start = i;
		ft_printf("[[green]] label[[end]]\n");
	}
	if (begin_state == 5 && (end_state == 1 || end_state == 4 || end_state == 5))
	{
		*command_start = i;
		ft_printf("[[green]] arg[[end]]\n");
	}
	*command_start = i;
}

void	lexe_one_token(int i, char *line)
{
	int begin_state;
	int end_state;
	int command_start;

	end_state = 1;
	begin_state = 1;
	command_start = i;
	while (begin_state)
	{
		begin_state = end_state;
		end_state = g_initial_parser[begin_state][g_alpha[(int)line[i]]][0];
		if (begin_state != end_state)
			save_tokens(line, &command_start, i, begin_state, end_state);
		i += g_initial_parser[begin_state][g_alpha[(int)line[i]]][1];
		if (end_state == 1)
			command_start = i;
	}
}



void	read_instructions(t_info *info, t_instruction instructions)
{
	(void)instructions;
	(void)info;
	lexe_one_token(info->read_pos, info->file);
}


// check at the beginning if integers are of size 4 (always the case)
int		main(int ac, char **av)
{
	t_fd			fd;
	t_info			info;
	t_instruction	instructions;

	fd.read = -1;
	fd.write = -1;
	instructions = set_instructions(); 
	info.label_info.label_list = NULL;
	info.label_info.label_categories = LABEL_INITIAL_NBR;
	info.label_info.n = 0;
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
//	ft_printf("%s\n", &info.file[info.read_pos]);
	read_instructions(&info, instructions);
//	ft_printf("[[red]]%d %d\n[[end]]", ',', g_alpha[44]);
	/*
	check_label_list(&(info.label_info));
	input_labels(&(info.label_info), &info);
	write_file(fd, &info);
	*/
	close(fd.read);
//	sleep(45);
}
