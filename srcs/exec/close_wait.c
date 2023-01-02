/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_wait.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 15:40:09 by ncotte            #+#    #+#             */
/*   Updated: 2022/12/30 15:40:11 by ncotte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_file(int i)
{
	if (i != STDIN_FILENO && i != STDOUT_FILENO && i != STDERR_FILENO)
		close(i);
}

static void	wait_child(int index, int nb_process, int pid)
{
	int	last_pid;
	int	status;

	signals();
	last_pid = waitpid(pid, &status, 0);
	if (last_pid < 0 && errno != ECHILD)
		perror("WAIT ERROR");
	if (index == nb_process - 1 && g_var.quit_child == NO)
	{
		if (WIFEXITED(status))
			g_var.last_er = WEXITSTATUS(status);
		if (WIFSIGNALED(status))
			g_var.last_er = WTERMSIG(status) + 128;
		if (WIFSTOPPED(status))
			g_var.last_er = WSTOPSIG(status);
	}
}

void	close_wait(t_pipes pipes, int nb_process, int *pids)
{
	int	i;

	close_file(pipes.files[0]);
	close_file(pipes.files[1]);
	close_file(pipes.in[0]);
	close_file(pipes.in[1]);
	close_file(pipes.out[0]);
	close_file(pipes.out[1]);
	i = -1;
	while (++i < nb_process)
	{
		if (pids[i] < EXCEPTION)
			g_var.last_er = pids[i] * -1;
		else if (pids[i] != EXCEPTION)
			wait_child(i, nb_process, pids[i]);
	}
	free(pids);
}
