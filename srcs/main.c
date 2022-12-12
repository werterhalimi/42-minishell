/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 12:56:18 by ncotte            #+#    #+#             */
/*   Updated: 2022/12/12 22:20:25 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*var_value(char const *var_name)
{
	t_list	*list;
	int		length;

	list = g_var.envp;
	length = ft_strlen(var_name);
	while (list && ft_strncmp(list->content, var_name, length))
		list = list->next;
	if (list)
		return (list->content + length + 1);
	return ("");
}

int	execute(t_command instr, char *envp[])
{
	char	**paths;
	char	**command;
	char	*path;
	int		pid;

	if (!ft_strncmp(instr.command, "exit", 4))
		return (-1);
	if (!ft_strncmp(instr.command, "env", 3))
		return (env());
	if (!ft_strncmp(instr.command, "pwd", 3))
		return (pwd());
	if (!ft_strncmp(instr.command, "export", 6))
		return (export(instr.args[1]));
	if (!ft_strncmp(instr.command, "unset", 5))
		return (unset(instr.args[1]));
	
	pid = fork();
	if (!pid)
	{
		paths = env_to_paths(envp);
		command = instr.args;
		path = get_path(paths, instr.command);
		execve(path, command, envp);
	}
	wait(&pid);

	return (0);
}

int	main(int argc, char *argv[], char *envp[])
{
	char		*buf = "";
	t_command	command;

	(void) argv;
	if (argc != 1)
		return (print_error("Invalid number of arguments"));
	if (init(envp))
		return (free_all(NULL));
	while (1)
	{
		buf = readline(var_value("PROMPT"));
		command = parse(buf);
		if (buf && *buf)
		{
			add_history(buf);
			if (execute(command, envp) == -1)
				return (free_all(buf));
		}
		free_buffer(g_var.parse_alloc);
	}
}
