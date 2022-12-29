/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 12:56:18 by ncotte            #+#    #+#             */
/*   Updated: 2022/12/28 18:47:21 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_global	g_var;

int	execute(t_command instr)
{
	int	ret;

	ret = ERROR;
	if (!ft_strncmp(instr.command, "exit", 4))
		ret = ft_exit();
	else if (!ft_strncmp(instr.command, "env", 3))
		ret = env();
	else if (!ft_strncmp(instr.command, "pwd", 3))
		ret = pwd();
	else if (!ft_strncmp(instr.command, "export", 6))
		ret = export(instr.args[1]);
	else if (!ft_strncmp(instr.command, "unset", 5))
		ret = unset(instr.args[1]);
	else if (!ft_strncmp(instr.command, "cd", 2))
		ret = cd(instr.args);
	else if (!ft_strncmp(instr.command, "echo", 4))
		ret = echo(instr.args);
	return (ret);
}

#ifndef UNIT

int	main(int argc, char *argv[], char *envp[])
{
	char	*buf;
	char	*tmp;

	(void) argv;
	if (argc != 1)
		return (print_error("Invalid number of arguments"));
	if (init(envp))
		return (free_all(NULL));
	buf = "";
	while (!g_var.exit)
	{
		signals();
		buf = readline(var_value("PROMPT"));
		if (!buf)
			break ;
		tmp = buf;
		buf = ft_strtrim(tmp, " 	");
		free(tmp);
		if (*buf)
		{
			add_history(buf);
			launch_pipex(ft_countchar(buf, '|'), \
				ft_split(buf, '|'), (int [2]){0, 1});
		}
		free_buffer(buf);
	}
	return (free_all(buf));
}
#endif 

#ifdef UNIT

int	main(void)
{
	int	i;

	i = 0;
	printf("%i", i);
}
#endif
