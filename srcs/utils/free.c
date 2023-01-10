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

void	free_cmd(t_command cmd)
{
	int	i;

	i = 0;
	while (i < cmd.len)
		free(cmd.args[i++]);
	free(cmd.args);
}

void	free_array(char *argv[])
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
		free(argv[i++]);
	free(argv);
}

int	free_buffer(char *buf)
{
	if (!g_var.exit)
		free(buf);
	ft_putstr_fd(RESET_COLOR, STDERR_FILENO);
	return (g_var.last_er);
}

int	free_all(char *buf, struct termios *termios_copy)
{
	if (!g_var.exit)
		ft_putendl_fd("exit", STDOUT_FILENO);
	free_array(g_var.envp);
	rl_clear_history();
	if (termios_copy)
		tcsetattr(STDIN_FILENO, TCSANOW, termios_copy);
	g_var.exit = NO;
	return (free_buffer(buf));
}
