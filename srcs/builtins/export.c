/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 16:34:05 by ncotte            #+#    #+#             */
/*   Updated: 2022/12/08 16:34:07 by ncotte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	export(char *str)
{
	int		i;
	t_list	*list;

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
			list->content = str;
		}
		else if (list && !list->next)
		{
			list->next = ft_lstnew(str);
			if (!list->next)
				return (1);
		}
		else
			return (1);
	}
	return (0);
}
