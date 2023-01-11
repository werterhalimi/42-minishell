/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/21 11:49:24 by ncotte            #+#    #+#             */
/*   Updated: 2022/12/21 11:49:25 by ncotte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// \brief Signal INT handler (Ctrl + C)
/// \n when reading the user input commands
/// \param code the signal code
static void	sig_int_read(int code)
{
	(void)code;
	write(STDERR_FILENO, "\n", 1);
	if (isatty(STDIN_FILENO))
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	g_var.last_er = ERROR;
}

/// \brief Signal INT handler (Ctrl + C)
/// \n when executing commands
/// \param code the signal code
static void	sig_int_exec(int code)
{
	(void)code;
	write(STDERR_FILENO, "\n", 1);
}

/// \brief Signal QUIT handler (Ctrl + \)
/// \n when executing commands
/// \param code the signal code
static void	sig_quit_exec(int code)
{
	(void)code;
	kill(g_var.pid, SIGINT);
	ft_putendl_fd("Quit: 3", STDERR_FILENO);
	if (g_var.quit_child == NO)
		g_var.last_er = QUIT_CHILD;
	g_var.quit_child = YES;
}

/// \brief Signal INT handler (Ctrl + C)
/// \n when redirecting STDIN to the first process
/// \param code the signal code
static void	sig_int_heredoc(int code)
{
	(void)code;
	write(STDERR_FILENO, "\n", 1);
	g_var.last_er = ERROR;
	g_var.quit_child = YES;
	g_var.status = EXECUTE;
}

void	signals(void)
{
	if (g_var.status == EXECUTE && g_var.pid)
	{
		signal(SIGINT, sig_int_exec);
		signal(SIGQUIT, sig_quit_exec);
	}
	else if (g_var.status == EXECUTE)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	else
	{
		signal(SIGQUIT, SIG_IGN);
		if (g_var.status == READ)
			signal(SIGINT, sig_int_read);
		else if (g_var.status == HEREDOC && !g_var.pid)
			signal(SIGINT, SIG_DFL);
		else
		{
			signal(SIGINT, sig_int_heredoc);
			wait(NULL);
		}
	}
}
