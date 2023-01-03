/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 16:34:20 by ncotte            #+#    #+#             */
/*   Updated: 2022/12/08 17:35:45 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd(char *argv[])
{
	int		i;
	char	*tmp;

	i = 0;
	(void) argv;
	while (g_var.envp[i] && ft_strncmp(g_var.envp[i], "PWD=", 4))
		i++;
	if (g_var.envp[i])
		ft_putendl_fd(g_var.envp[i] + 4, STDOUT_FILENO);
	else
	{
		tmp = getcwd(NULL, 0);
		if (!tmp)
			return (ERROR);
		ft_putendl_fd(tmp, STDOUT_FILENO);
		free(tmp);
	}
	return (SUCCESS);
}
