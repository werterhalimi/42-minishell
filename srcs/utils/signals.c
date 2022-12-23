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
static void	sig_int(int code)
{
	(void)code;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_var.last_er = 1;
//	g_var.sigint = 1;
}

static void	sig_child(int code)
{
	(void)code;
	kill(g_var.pid, SIGINT);
	g_var.last_er = 131;
}

/*
//	Ctrl + "\"
void	sig_quit(int code)
{
	(void)code;
//	if (g_var.pid)
//		*g_var.last_er = 131;
//	else
//		ft_putstr_fd("\b\b ", STDERR_FILENO);
//	rl_on_new_line();
//	rl_replace_line("", 0);
//	ft_putstr_fd("\b\b", STDERR_FILENO);
//	rl_on_new_line();
//	rl_redisplay();
}
*/

void	signals(void)
{
	signal(SIGTERM, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_DFL);
	if (g_var.status == READ)
		signal(SIGINT, sig_int);
	else if (g_var.pid)
		signal(SIGINT, sig_child);
}
