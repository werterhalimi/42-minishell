/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 12:56:18 by ncotte            #+#    #+#             */
/*   Updated: 2022/12/08 17:35:44 by shalimi          ###   ########.fr       */
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

int	execute(char *instr)
{
	if (!ft_strncmp(instr, "exit", 4))
		return (-1);
	if (!ft_strncmp(instr, "env", 3))
		return (env());
	if (!ft_strncmp(instr, "pwd", 3))
		return (pwd());
	if (!ft_strncmp(instr, "export", 6))
		return (export(instr + 7));
	if (!ft_strncmp(instr, "unset", 5))
		return (unset(instr + 6));
	return (0);
}

int	main(int argc, char *argv[], char *envp[])
{
	char	*buf;

	(void) argv;
	if (argc != 1)
		return (print_error("Invalid number of arguments"));
	if (init(envp))
		return (free_all(NULL));
	while (1)
	{
		buf = readline(var_value("PROMPT"));
		if (buf && *buf)
		{
			add_history(buf);
			if (execute(buf) == -1)
				return (free_all(buf));
		}
		free(buf);
	}
}
