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
		return ("> ");
	prompt2 = ft_strjoin(prompt1, "minishell > ");
	free(prompt1);
	if (!prompt2)
		return ("> ");
	prompt1 = ft_strjoin(prompt2, RESET_COLOR);
	free(prompt2);
	if (!prompt1)
		return ("> ");
	return (prompt1);
}

int	init(char *envp[])
{
	int		i;
	char	*tmp;

	i = 0;
	g_var.exit = 0;
	g_var.status = READ;
	g_var.pid = 0;
	g_var.exec = SUCCESS;
//	g_var.sigint = 0;
	while (envp[i])
		i++;
	g_var.envp = array_copy(envp, i);
	if (!g_var.envp)
		return (ERROR);
	unset("OLDPWD");
	export("OLDPWD");
	tmp = prompt();
	i = export(tmp);
	free(tmp);
	return (i);
}
