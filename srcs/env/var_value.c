/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_value.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 10:39:36 by ncotte            #+#    #+#             */
/*   Updated: 2022/12/20 10:39:39 by ncotte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*var_value(char const *var_name)
{
	t_list	*list;
	size_t	length;

	list = g_var.envp;
	length = ft_strlen(var_name);
	while (list && ft_strncmp(list->content, var_name, length))
		list = list->next;
	if (list)
		return (list->content + length + 1);
	return ("");
}
