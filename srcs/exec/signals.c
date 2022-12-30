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

//	Ctrl + C
static void	sig_int_main(int code)
{
	(void)code;
	write(STDERR_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_var.last_er = ERROR;
}

static void	sig_int_child(int code)
{
	(void)code;
	write(STDERR_FILENO, "\n", 1);
}

static void	sig_quit_child(int code)
{
	(void)code;
	kill(g_var.pid, SIGINT);
	ft_putendl_fd("Quit: 3", STDERR_FILENO);
	g_var.last_er = QUIT_CHILD;
	g_var.quit_child = YES;
}

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
	int	status;

	signal(SIGTERM, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_DFL);
	if (g_var.status == READ)
		signal(SIGINT, sig_int_main);
	else if (g_var.status == HEREDOC && g_var.pid)
	{
		signal(SIGINT, sig_int_heredoc);
		wait(&status);
	}
	else if (g_var.pid)
	{
		signal(SIGINT, sig_int_child);
		signal(SIGQUIT, sig_quit_child);
	}
}
