/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 16:44:10 by ncotte            #+#    #+#             */
/*   Updated: 2022/12/30 17:37:35 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_envp(void)
{
	int	i;

	if (!g_var.envp)
		return ;
	i = 0;
	while (g_var.envp[i])
		free(g_var.envp[i++]);
	free(g_var.envp);
}

int	free_all(char *buf)
{
	if (!g_var.exit)
		ft_putendl_fd("exit", STDOUT_FILENO);
	free_envp();
	rl_clear_history();
	return (free_buffer(buf));
}

int	free_buffer(char *buf)
{
	ft_lstclear(g_var.parse_alloc, free);
	if (!g_var.exit)
		free(buf);
	ft_putstr_fd(RESET_COLOR, STDERR_FILENO);
	return (g_var.last_er);
}

void	free_cmd(t_command cmd)
{
	int	i;

	i = 0;
	while (i < cmd.len)
	{
		free(cmd.args[i]);
		i++;
	}
	free(cmd.args);
}
