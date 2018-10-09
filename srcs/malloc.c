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
