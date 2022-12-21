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
void	sig_int(int code)
{
	(void)code;
//	if ()
//	ft_putstr_fd("\b\b", STDOUT_FILENO);
	ft_printf("\b\b\n");
	rl_on_new_line();
	rl_replace_line("", 0);
//	ft_putstr_fd("\n", STDOUT_FILENO);
	rl_redisplay();
//	*g_var.last_er = 1;
	g_var.sigint = 1;
}

//	Ctrl + "\"
void	sig_quit(int code)
{
	(void)code;
//	if (g_var.pid)
//		*g_var.last_er = 131;
//	else
//		ft_putstr_fd("\b\b ", STDERR_FILENO);
//	rl_on_new_line();
	ft_putstr_fd("\b\b", STDERR_FILENO);
	rl_on_new_line();
	rl_redisplay();
}

