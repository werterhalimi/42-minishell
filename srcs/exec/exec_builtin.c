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
	return (!ft_strncmp(cmd, "echo", 5) \
		|| !ft_strncmp(cmd, "cd", 3) \
		|| !ft_strncmp(cmd, "pwd", 4) \
		|| !ft_strncmp(cmd, "export", 7) \
		|| !ft_strncmp(cmd, "unset", 6) \
		|| !ft_strncmp(cmd, "env", 4) \
		|| !ft_strncmp(cmd, "exit", 5));
}

int	exec_builtin(t_command instr)
{
	if (!ft_strncmp(instr.command, "exit", 5))
		return (ft_exit(instr.args));
	if (!ft_strncmp(instr.command, "env", 4))
		return (env(instr.args[1]));
	if (!ft_strncmp(instr.command, "pwd", 5))
		return (pwd());
	if (!ft_strncmp(instr.command, "export", 7))
		return (export(instr.args));
	if (!ft_strncmp(instr.command, "unset", 6))
		return (unset(instr.args));
	if (!ft_strncmp(instr.command, "cd", 3))
		return (cd(instr.args));
	if (!ft_strncmp(instr.command, "echo", 5))
		return (echo(instr.args));
	return (ERROR);
}
