#include "../includes/asm.h"
#include "../libft/includes/libft.h"
#include <stdlib.h>

static t_label	*new_malloced_label(char *name, int is_anchor, int position)
{
	t_label *new;

	if (!(new = malloc(sizeof(t_label))))
		ft_myexit("malloc error");
	new->name = NULL;
	if (!(new->name = ft_strdup(name)))
		ft_myexit("malloc error");
	new->next = NULL;
	new->position = position;
	new->anchor = (is_anchor) ? 1 : 0;
	return (new);
}

static t_label new_label(char *name, int is_anchor, int position)
{
	t_label new;

	new.name = NULL;
	if (!(new.name = ft_strdup(name)))
		ft_myexit("malloc error");
	new.next = NULL;
	new.position = position;
	new.anchor = (is_anchor) ? 1 : 0;
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
	int				position;

	to_swap->anchor = 0;
	(info->label_list)[i].anchor = 1;
	position = to_swap->position;
	to_swap->position = (info->label_list)[i].position;
	(info->label_list)[i].position = position;
}

void	add_label_to_list(t_label_info *info, char *label, int is_anchor, int position)
{
	int			i;
	int			strlen_cmp;
	int			strlen_new;
	t_label		*tmp;

	i = -1;
	strlen_new = ft_strlen(label);
	while (++i < info->n)
	{
		strlen_cmp = ft_strlen((info->label_list)[i].name);
		if (strlen_cmp == strlen_new
			&& !ft_memcmp((info->label_list)[i].name, label, strlen_new))
		{
			tmp = &(info->label_list)[i];
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = new_malloced_label(label, is_anchor, position);
			if (((t_label*)tmp->next)->anchor)
				put_anchor_first(info, i, ((t_label*)tmp->next));
			return ;
		}
	}
	add_new_label_type(info, new_label(label, is_anchor, position), i);
}

void	label_list(t_label_info *info, char	*label, int is_anchor, int position)
{
	if (!(info->label_list))
		create_label_list(info);
	add_label_to_list(info, label, is_anchor, position);
}

void	check_label_list(t_label_info *info)
{
	int			i;

	i = 0;
	while (i < info->n)
	{
		if (!(info->label_list[i].anchor))
			ft_myexit(ft_strjoin("invalid label : ", info->label_list[i].name));
		i++;
	}
}

void	input_labels(t_label_info *label_info, t_info *info)
{
	int		i;
	t_label	tmp;
	int		pos;
	short	distance;

	i = -1;
	while (++i < label_info->n)
	{
		pos = (label_info->label_list)[i].position;
		if ((label_info->label_list)[i].next)
		{
			tmp = *(t_label*)((label_info->label_list)[i].next);
			distance = 1 + pos - tmp.position;
			if (!ft_check_little_endianness())
				distance = little_endian_to_big(distance, sizeof(short));
			(info->to_write)[tmp.position] = distance;
			while (tmp.next)
			{
				tmp = *(t_label*)((label_info->label_list)[i].next);
				distance = tmp.position - pos;
				(info->to_write)[tmp.position] = distance;
				tmp = *(t_label*)tmp.next;
			}
		}
	}
}
