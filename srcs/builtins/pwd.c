/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 16:34:20 by ncotte            #+#    #+#             */
/*   Updated: 2022/12/08 16:34:21 by ncotte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	pwd(void)
{
	t_list	*list;

	list = g_var.envp;
	while (list && ft_strncmp(list->content, "PWD=", 4))
		list = list->next;
	if (list)
		ft_putendl_fd(list->content + 4, STDOUT_FILENO);
	return (0);
}
