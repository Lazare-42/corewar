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

t_label new_label(int pos_in_file, int len, int label_pos, int write_pos, int byte_size)
{
	t_label new;

	new.pos_in_file = pos_in_file;
	new.name_len = len;
	new.byte_size = byte_size;
	new.label_pos = label_pos;
	new.write_pos = write_pos;
	new.next = NULL;
	return (new);
}

void	create_label_list(t_label_info *info)
{
	unsigned int i;
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
void	add_label_to_list(t_info *info, t_label new)
{
	unsigned int			i;
	unsigned int			strlen_cmp;
	t_label					*tmp;

	i = -1;
	while (++i < info->label_info.n)
	{
		strlen_cmp = info->label_info.label_list[i].name_len;
		if (strlen_cmp == new.name_len && !ft_memcmp(&info->file[info->label_info.label_list[i].pos_in_file], &info->file[new.pos_in_file], new.name_len))
		{
			tmp = &(info->label_info.label_list)[i];
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = new_malloced_label(new);
			if (!((t_label*)tmp->next)->write_pos)
				put_anchor_first(&(info->label_info), i, ((t_label*)tmp->next));
			return ;
		}
	}
	add_new_label_type(&(info->label_info), new, i);
}

void	label_list(t_info *info, t_label new)
{
	if (!(info->label_info.label_list))
		create_label_list(&(info->label_info));
	add_label_to_list(info, new);
}

void	check_label_list(t_label_info *info)
{
	unsigned int			i;

	i = 0;
	while (i < info->n)
	{
		if (info->label_list[i].write_pos)
			ft_myexit("unable to dereference label : ");
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
	unsigned int		i;
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

#include <unistd.h>
void		print_label_list(t_info *info)
{
	unsigned int i;
	t_label	*tmp;

	i = 0;
	while (i < info->label_info.n)
	{
		ft_printf("[[blue]]");
		write(1, &info->file[info->label_info.label_list[i].pos_in_file], info->label_info.label_list[i].name_len);
		tmp = info->label_info.label_list[i].next;
		while (tmp)
		{
			ft_printf("[[cyan]]->");
			write(1, &info->file[tmp->pos_in_file], tmp->name_len);
			tmp = tmp->next;
		}
		ft_printf("[[end]]\n");
		i++;
	}
}
