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
		*buf = readline(var_value("PROMPT"));
		if (!(*buf))
			break ;
		tmp = *buf;
		*buf = ft_strtrim(tmp, " 	");
		free(tmp);
		if (**buf)
		{
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
	char			*buf;
	struct termios	termios_new;
	struct termios	termios_copy;

	(void) argv;
	if (argc != 1)
		return (print_custom_error("Invalid number of arguments"));
	if (init(envp, &termios_new, &termios_copy))
		return (print_custom_error("Error during initialization"));
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
