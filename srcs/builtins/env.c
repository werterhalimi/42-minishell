/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 16:34:13 by ncotte            #+#    #+#             */
/*   Updated: 2022/12/08 16:34:15 by ncotte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	env(void)
{
	t_list	*list;

	list = g_var.envp;
	while (list)
	{
		ft_putendl_fd(list->content, STDOUT_FILENO);
		list = list->next;
	}
	return (0);
}
