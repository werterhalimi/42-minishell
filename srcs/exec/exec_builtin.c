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
	int	ret;

	ret = ERROR;
	if (!ft_strncmp(instr.command, "exit", 5))
		ret = ft_exit(instr.args);
	else if (!ft_strncmp(instr.command, "env", 4))
		ret = env(instr.args[1]);
	else if (!ft_strncmp(instr.command, "pwd", 5))
		ret = pwd();
	else if (!ft_strncmp(instr.command, "export", 7))
		ret = export(instr.args[1]);
	else if (!ft_strncmp(instr.command, "unset", 6))
		ret = unset(instr.args[1]);
	else if (!ft_strncmp(instr.command, "cd", 3))
		ret = cd(instr.args);
	else if (!ft_strncmp(instr.command, "echo", 5))
		ret = echo(instr.args);
	return (ret);
}
