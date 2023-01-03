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

void	close_file(int fd)
{
	if (fd != STDIN_FILENO && fd != STDOUT_FILENO && fd != STDERR_FILENO)
		close(fd);
}

/// \brief Wait all child process and set the last error code ($?) accordingly
/// \param last_pid the pid of the last child process
static void	wait_child(int last_pid)
{
	int	current_pid;
	int	status;

	signals();
	current_pid = wait(&status);
	if (current_pid < 0 && errno != ECHILD)
		perror("WAIT ERROR");
	if (last_pid == current_pid && g_var.quit_child == NO)
	{
		if (WIFEXITED(status))
			g_var.last_er = WEXITSTATUS(status);
		if (WIFSIGNALED(status))
			g_var.last_er = WTERMSIG(status) + 128;
		if (WIFSTOPPED(status))
			g_var.last_er = WSTOPSIG(status);
		g_var.quit_child = YES;
	}
}

void	close_wait(t_pipes pipes, int nb_process, int *pids)
{
	int	i;
	int	last_pid;

	close_file(pipes.files[0]);
	close_file(pipes.files[1]);
	close_file(pipes.in[0]);
	close_file(pipes.in[1]);
	close_file(pipes.out[0]);
	close_file(pipes.out[1]);
	i = -1;
	last_pid = pids[nb_process - 1];
	while (++i < nb_process)
	{
		if (pids[i] < EXCEPTION)
			g_var.last_er = pids[i] * -1;
		else if (pids[i] != EXCEPTION)
			wait_child(last_pid);
	}
	free(pids);
}
