#include "../includes/asm.h"
#include "../libft/includes/libft.h"
#include <stdlib.h>

static t_label	*new_malloced_label(t_label to_store)
{
	t_label *new;

	if (!(new = malloc(sizeof(t_label))))
		ft_myexit("malloc error");
	*new = to_store;
	return (new);
}

t_label new_label(char *name, int label_pos, int write_pos, int byte_size)
{
	t_label new;

	new.name = NULL;
	if (!(new.name = ft_strdup(name)))
		ft_myexit("malloc error in new_label");
	new.byte_size = byte_size;
	new.next = NULL;
	new.label_pos = label_pos;
	new.write_pos = write_pos;
	return (new);
}

void	create_label_list(t_label_info *info)
{
	int i;
	t_label	*new_label_list;

	new_label_list = NULL;
	if (!(new_label_list = malloc(sizeof(t_label) * info->label_categories)))
		ft_myexit("malloc error in create_label_list");
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

void	add_new_label_type(t_label_info *info, t_label new, int i)
{
	info->n++;
	if (info->n == info->label_categories)
	{
		info->label_categories *= 2;
		create_label_list(info);
	}
	(info->label_list)[i] = new;
}

void	put_anchor_first(t_label_info *info, int i, t_label *to_swap)
{
	int				label_pos;
	int				write_pos;

	label_pos = to_swap->label_pos;
	write_pos = to_swap->write_pos;
	to_swap->write_pos = (info->label_list)[i].write_pos;
	to_swap->label_pos = (info->label_list)[i].label_pos;
	to_swap->byte_size = (info->label_list)[i].byte_size;
	(info->label_list)[i].write_pos = write_pos;
	(info->label_list)[i].label_pos = label_pos;
}

void	add_label_to_list(t_label_info *info, t_label new)
{
	int			i;
	int			strlen_cmp;
	int			strlen_new;
	t_label		*tmp;

	i = -1;
	strlen_new = ft_strlen(new.name);
	while (++i < info->n)
	{
		strlen_cmp = ft_strlen((info->label_list)[i].name);
		if (strlen_cmp == strlen_new
			&& !ft_memcmp((info->label_list)[i].name, new.name, strlen_new))
		{
			tmp = &(info->label_list)[i];
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = new_malloced_label(new);
			if (!((t_label*)tmp->next)->write_pos)
				put_anchor_first(info, i, ((t_label*)tmp->next));
			return ;
		}
	}
	add_new_label_type(info, new, i);
}

void	label_list(t_label_info *info, t_label new)
{
	if (!(info->label_list))
		create_label_list(info);
	add_label_to_list(info, new);
}

void	check_label_list(t_label_info *info)
{
	int			i;

	i = 0;
	while (i < info->n)
	{
		if (info->label_list[i].write_pos)
			ft_myexit(ft_strjoin("invalid label : ", info->label_list[i].name));
		i++;
	}
}

void	write_label(t_info *info, int where, short distance, int byte_size)
{
	int	distance_int;

	if (byte_size == sizeof(short))
	{
		if (ft_check_little_endianness())
			distance = little_endian_to_big(distance, sizeof(short));
		ft_memcpy((void*)&info->to_write[where], &distance, sizeof(short));
		return ;
	}
	else if (byte_size == sizeof(int))
	{
		distance_int = distance;
		if (ft_check_little_endianness())
			distance_int = little_endian_to_big(distance_int, sizeof(int));
		ft_memcpy((void*)&info->to_write[where], &distance_int, sizeof(int));
	}
}

void	input_labels(t_label_info *label_info, t_info *info)
{
	int		i;
	t_label	*tmp;
	int		pos;
	short	distance;

	i = -1;
	while (++i < label_info->n)
	{
		pos = label_info->label_list[i].label_pos;
		tmp = label_info->label_list[i].next;
		while (tmp)
		{
			distance = pos - tmp->label_pos;
			write_label(info, tmp->write_pos, distance, tmp->byte_size);
			tmp = tmp->next;
		}
	}
}
