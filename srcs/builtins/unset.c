/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 16:34:31 by ncotte            #+#    #+#             */
/*   Updated: 2022/12/08 16:34:32 by ncotte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	unset(char *str)
{
	int		length;
	t_list	*list;
	t_list	*tmp;

	length = ft_strlen(str);
	list = g_var.envp;
	if (list && !ft_strncmp(list->content, str, length))
	{
		g_var.envp = list->next;
		ft_lstdelone(list, free);
		return (0);
	}
	while (list && list->next && !ft_strncmp(list->next->content, str, length))
		list = list->next;
	if (list && list->next)
	{
		tmp = list->next;
		list->next = tmp->next;
		ft_lstdelone(tmp, free);
		return (0);
	}
	return (1);
}
