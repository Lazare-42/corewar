#include "../includes/asm.h"
#include "../libft/includes/libft.h"
#include <stdlib.h>

static t_label	*new_malloced_label(char *name, int is_anchor)
{
	t_label *new;

	if (!(new = malloc(sizeof(t_label))))
		ft_myexit("malloc error");
	new->name = NULL;
	if (!(new->name = ft_strdup(name)))
		ft_myexit("malloc error");
	new->next = NULL;
	new->position = 0;
	new->anchor = (is_anchor) ? 1 : 0;
	return (new);
}

static t_label new_label(char *name, int is_anchor)
{
	t_label new;

	new.name = NULL;
	if (!(new.name = ft_strdup(name)))
		ft_myexit("malloc error");
	new.next = NULL;
	new.position = 0;
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

void	add_label_to_list(t_label_info *info, char *label, int is_anchor)
{
	int			i;
	int			strlen_cmp;
	int			strlen_new;
	t_label		*tmp;
	t_label		new;

	i = 0;
	strlen_new = ft_strlen(label);
	while (i < info->n)
	{
		strlen_cmp = ft_strlen((info->label_list)[i].name);
		if (strlen_cmp == strlen_new && !ft_memcmp((info->label_list)[i].name, label, strlen_new))
		{
			tmp = &(info->label_list)[i];
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = new_malloced_label(label, is_anchor);
			return ;
		}
		i++;
	}
	new = new_label(label, is_anchor);
	info->n++;
	if (info->n == info->label_categories)
	{
		info->label_categories *= 2;
		create_label_list(info);
	}
	(info->label_list)[i] = new;
}

void	label_list(t_label_info *info, char	*label, int is_anchor)
{
	if (!(info->label_list))
		create_label_list(info);
	add_label_to_list(info, label, is_anchor);
}

void	print_label_list(t_label_info *info)
{
	int			i;
	t_label		tmp;

	i = 0;
	while (i < info->n)
	{
		ft_printf("[[green]] %s[[end]]", (info->label_list)[i].name);
		if ((info->label_list)[i].next)
		{
			tmp = *(t_label*)((info->label_list)[i].next);
			ft_printf("[[blue]] ->%10s[[end]]", tmp.name);
			while (tmp.next)
			{
				tmp = *(t_label*)tmp.next;
				ft_printf("[[blue]] ->%10s[[end]]", tmp.name);
			}
		}
		ft_printf("\n");
		i++;
	}
}
