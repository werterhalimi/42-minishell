/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 16:43:51 by ncotte            #+#    #+#             */
/*   Updated: 2022/12/08 17:35:45 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*prompt(void)
{
	char	*prompt1;
	char	*prompt2;

	prompt1 = ft_strjoin("PROMPT=", CYAN);
	if (!prompt1)
		return (NULL);
	prompt2 = ft_strjoin(prompt1, "minishell > ");
	free(prompt1);
	if (!prompt2)
		return (NULL);
	prompt1 = ft_strjoin(prompt2, RESET_COLOR);
	free(prompt2);
	return (prompt1);
}

static int	init_global_variable(char *envp[])
{
	int	i;

	g_var.exit = NO;
	g_var.status = READ;
	g_var.pid = 0;
	g_var.quit_child = NO;
	g_var.last_er = ERROR;
	i = 0;
	while (envp[i])
		i++;
	g_var.envp = array_copy(envp, i);
	if (!g_var.envp)
		return (ERROR);
	return (SUCCESS);
}

int	init(char *envp[])
{
	int		res;
	char	*tmp;

	if (init_global_variable(envp))
		return (free_all(NULL));
	if (unset("OLDPWD"))
		return (free_all(NULL));
	if (export("OLDPWD"))
		return (free_all(NULL));
	tmp = prompt();
	if (!tmp)
		return (free_all(NULL));
	res = export(tmp);
	free(tmp);
	if (res)
		return (free_all(NULL));
	g_var.last_er = SUCCESS;
	return (SUCCESS);
}
