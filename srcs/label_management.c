#include "../includes/asm.h"
#include "../libft/includes/libft.h"
#include <stdlib.h>

static t_label	*new_label(char *name, int is_anchor)
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

t_label		*label_list(char	*label, int is_anchor)
{
	static t_label	*label_list = NULL;
	t_label			*new;
	t_label			*tmp;

	tmp = label_list;
	ft_printf("[[blue]]this is label %s\n[[end]]", label);
	if (label)
		new = new_label(label, is_anchor);
	else
		return (label_list);
	if (!label_list)
		return (label_list = new);
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	return (label_list);
}
