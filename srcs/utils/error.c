/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 16:43:57 by ncotte            #+#    #+#             */
/*   Updated: 2022/12/08 17:35:45 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	errno_to_errcode(int errnum)
{
	if (errnum == EACCES)
		return (PERM_ERROR);
	if (errnum == ENOENT)
		return (PATH_ERROR);
	return (-1);
}

int	print_error(char *error_msg)
{
	ft_putstr_fd(RED, STDERR_FILENO);
	ft_putendl_fd(error_msg, STDERR_FILENO);
	return (ERROR);
}

int	print_errno(const char *cmd, const char *arg, int errnum)
{
	ft_printf_fd(STDERR_FILENO, "%s%s: %s: %s\n", RED, cmd, arg, strerror(errnum));
	return (errno_to_errcode(errnum));
}

int	print_current_errno(const char *cmd, const char *arg)
{
	ft_printf_fd(STDERR_FILENO, "%s%s: ", RED, cmd);
	perror(arg);
	return (errno_to_errcode(errno));
}
