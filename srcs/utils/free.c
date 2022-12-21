/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 16:44:10 by ncotte            #+#    #+#             */
/*   Updated: 2022/12/22 00:39:19 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_envp(void)
{
	int	i;

	i = 0;
	while (g_var.envp[i])
		free(g_var.envp[i++]);
	free(g_var.envp);
}

int	free_all(char *buf)
{
	(void) buf;
	free_envp();
	rl_clear_history();
	return (0);
}

int	free_buffer(char *buf)
{
	g_var.sigint = 0;
	ft_lstclear(g_var.parse_alloc, free);
	free(buf);
	return (0);
}
