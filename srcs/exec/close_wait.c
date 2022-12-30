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

void	close_wait(int fd[2], int out[2], int j, int *pid)
{
	int	i;
	int	status;
	int	last_pid;

	close_file(fd[0]);
	close_file(fd[1]);
	close_file(out[0]);
	close_file(out[1]);
	i = -1;
	while (++i < j)
	{
		if (pid[i] < NO_WAIT)
			g_var.last_er = pid[i] * -1;
		else if (pid[i] != NO_WAIT)
		{
			signals();
			last_pid = waitpid(pid[i], &status, 0);
			if (last_pid < 0 && errno != ECHILD) // TODO
				perror("WAIT ERROR");
			if (i == j - 1 && g_var.quit_child == NO)
			{
				if (WIFEXITED(status))
					g_var.last_er = WEXITSTATUS(status);
				if (WIFSIGNALED(status))
					g_var.last_er = WTERMSIG(status) + 128;
				if (WIFSTOPPED(status))
					g_var.last_er = WSTOPSIG(status);
			}
		}
	}
	free(pid);
}
