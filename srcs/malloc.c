#include "../includes/asm.h"
#include "../libft/includes/libft.h"
#include <stdlib.h>

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

void	malloc_resize_label_list(t_label_info *info)
{
	unsigned int i;
	t_label	*new_label_list;

	new_label_list = NULL;
	if (!(new_label_list = malloc(sizeof(t_label) * info->label_categories)))
		ft_myexit("malloc error in malloc_resize_label_list");
	if (info->label_list)
	{
		i = 0;
		while (i < info->label_categories / 2)
		{
			new_label_list[i] = info->label_list[i];
			i++;
		}
		ft_memdel((void**)&(info->label_list));
	}
	info->label_list = new_label_list;
}

t_label	*new_malloced_label(t_label to_store)
{
	t_label *new;

	if (!(new = malloc(sizeof(t_label))))
		ft_myexit("malloc error");
	*new = to_store;
	return (new);
}
