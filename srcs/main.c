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

void	print_cmd(t_command cmd)
{
	int	i;

	printf("\n++++++++++++++++++++++++++\n");
	printf("CMD : %s\n", cmd.command);
	i = -1;
	if (cmd.args)
	{
		printf("ARGS : %p\n", cmd.args);
		while (cmd.args[++i])
			printf("%d : %s\n", i, cmd.args[i]);
		printf("%d : %s\n", i, cmd.args[i]);
	}
	printf("fd[0] = %d\n", cmd.fd[0]);
	printf("fd[1] = %d\n", cmd.fd[1]);
	printf("++++++++++++++++++++++++++\n\n");
}

static void	main_loop(char **buf)
{
	char	*tmp;

	while (!g_var.exit)
	{
		signals();
		*buf = readline(var_value("PROMPT"));
		if (!(*buf))
			break ;
		tmp = *buf;
		*buf = ft_strtrim(tmp, " 	");
		free(tmp);
		if (*buf)
		{
			add_history(*buf);
			launch_pipex(ft_countchar(*buf, '|'), \
				ft_split(*buf, '|'), (int [2]){0, 1});
		}
		free_buffer(*buf);
	}
}

#ifndef UNIT

int	main(int argc, char *argv[], char *envp[])
{
	char	*buf;

	(void) argv;
	if (argc != 1)
		return (print_custom_error("Invalid number of arguments"));
	if (init(envp))
		return (print_custom_error("Error during initialization"));
	buf = "";
	main_loop(&buf);
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
