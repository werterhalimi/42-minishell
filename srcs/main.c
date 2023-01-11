/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 12:56:18 by ncotte            #+#    #+#             */
/*   Updated: 2023/01/03 21:17:12 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_global	g_var;

static void	main_loop(char **buf)
{
	char	*tmp;
	char	**commands;
	int		len;

	while (!g_var.exit)
	{
		signals();
		if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
			*buf = readline(var_value("PROMPT"));
		else
			*buf = get_next_line(STDIN_FILENO);
		if (!(*buf))
			return ;
		tmp = *buf;
		*buf = ft_strtrim(tmp, " \t\n");
		free(tmp);
		if (*buf && **buf)
		{
			if (isatty(STDIN_FILENO))
				add_history(*buf);
			commands = get_commands(*buf, '|', &len);
			main_exec(len, commands, (int [2]){0, 1});
		}
		free_buffer(*buf);
	}
}

#ifndef UNIT

int	main(int argc, char *argv[], char *envp[])
{
	char		*buf;
	t_termios	termios_new;
	t_termios	termios_copy;

	(void) argv;
	if (argc != 1)
		return (print_custom_error("Invalid number of arguments"));
	if (init(envp, &termios_new, &termios_copy))
		return (print_custom_error("Error during initialization"));
	if (!isatty(STDOUT_FILENO) && isatty(STDIN_FILENO))
		ft_printf_fd(STDERR_FILENO, "%s%s\n%s%s\n", BOLD_MAGENTA, \
			"Warning : the standard output is not a terminal.", \
			"The prompt and the history are disabled.", RESET_COLOR);
	buf = "";
	main_loop(&buf);
	return (free_all(buf, &termios_copy));
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
