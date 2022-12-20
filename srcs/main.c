/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 12:56:18 by ncotte            #+#    #+#             */
/*   Updated: 2022/12/20 19:33:25 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute(t_command instr)
{
	if (!ft_strncmp(instr.command, "exit", 4))
		return (ft_exit());
	if (!ft_strncmp(instr.command, "env", 3))
		return (env());
	if (!ft_strncmp(instr.command, "pwd", 3))
		return (pwd());
	if (!ft_strncmp(instr.command, "export", 6))
		return (export(instr.args[1]));
	if (!ft_strncmp(instr.command, "unset", 5))
		return (unset(instr.args[1]));
	if (!ft_strncmp(instr.command, "cd", 2))
		return (cd(instr.args));
	if (!ft_strncmp(instr.command, "echo", 4))
		return (echo(instr.args));
	return (0);
}

#ifndef UNIT
int	main(int argc, char *argv[], char *envp[])
{
	char		*buf = "";

	(void) argv;
	if (argc != 1)
		return (print_error("Invalid number of arguments"));
	if (init(envp))
		return (free_all(NULL));
	while (!g_var.exit)
	{
		buf = readline(var_value("PROMPT"));
		if (buf && *buf)
		{
			add_history(buf);
			launch_pipex(ft_countchar(buf, '|'), ft_split(buf, '|'), (int[2]) {0, 1});
			/*
			if (execute(command, envp) == -1)
				return (free_all(buf));*/
		}
		free_buffer(g_var.parse_alloc);
	}
	free_all(buf);
	return (SUCCESS);
}
#endif 

#ifdef UNIT

int	main(void)
{
	int i = 0;
	printf("%i", i);
}
#endif
