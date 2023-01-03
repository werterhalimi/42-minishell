/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 15:48:56 by ncotte            #+#    #+#             */
/*   Updated: 2022/12/30 15:48:58 by ncotte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isbuiltin(char *cmd)
{
	return (!ft_strncmp(cmd, "cd", 3) \
		|| !ft_strncmp(cmd, "echo", 5) \
		|| !ft_strncmp(cmd, "env", 4) \
		|| !ft_strncmp(cmd, "exit", 5) \
		|| !ft_strncmp(cmd, "export", 7) \
		|| !ft_strncmp(cmd, "pwd", 4) \
		|| !ft_strncmp(cmd, "unset", 6));
}

int	exec_builtin(t_command cmd)
{
	if (!ft_strncmp(cmd.command, "cd", 3))
		return (cd(cmd.args));
	if (!ft_strncmp(cmd.command, "echo", 5))
		return (echo(cmd.args));
	if (!ft_strncmp(cmd.command, "env", 4))
		return (env(cmd.args));
	if (!ft_strncmp(cmd.command, "exit", 5))
		return (ft_exit(cmd.args));
	if (!ft_strncmp(cmd.command, "export", 7))
		return (export(cmd.args));
	if (!ft_strncmp(cmd.command, "pwd", 5))
		return (pwd(cmd.args));
	if (!ft_strncmp(cmd.command, "unset", 6))
		return (unset(cmd.args));
	return (ERROR);
}
