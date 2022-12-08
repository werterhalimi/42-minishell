/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 16:34:05 by ncotte            #+#    #+#             */
/*   Updated: 2022/12/08 17:35:45 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	export(char *str)
{
	int		i;
	t_list	*list;
	char	*tmp;

	i = 0;
	list = g_var.envp;
	while (str[i] && str[i] != '=')
		i++;
	if (str[i])
	{
		while (list && list->next && ft_strncmp(list->content, str, i))
			list = list->next;
		if (list && list->next)
		{
			free(list->content);
			list->content = ft_strdup(str);
			if (!list->content)
				return (1);
		}
		else if (list && !list->next)
		{
			tmp = ft_strdup(str);
			if (!tmp)
				return (1);
			list->next = ft_lstnew(tmp);
			if (!list->next)
				return (1);
		}
		else
			return (1);
	}
	return (0);
}
