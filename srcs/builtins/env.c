/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 16:34:13 by ncotte            #+#    #+#             */
/*   Updated: 2022/12/08 17:35:45 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env(char *argv[])
{
	int	i;

	if (argv[1] && access(argv[1], F_OK))
		return (print_errno("env", argv[1], errno));
	else if (argv[1])
		return (print_errno("env", argv[1], EACCES));
	i = -1;
	while (g_var.envp[++i])
		if (ft_strchr(g_var.envp[i], '='))
			ft_putendl_fd(g_var.envp[i], STDOUT_FILENO);
	return (SUCCESS);
}
