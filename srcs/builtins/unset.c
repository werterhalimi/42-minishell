/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 16:34:31 by ncotte            #+#    #+#             */
/*   Updated: 2022/12/08 17:35:45 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unset(char *str)
{
	int		i;
	int		j;
	size_t	length;
	char	**tmp;

	i = 0;
	length = ft_strlen(str);
	while (g_var.envp[i] && ft_strncmp(g_var.envp[i], str, length))
		i++;
	if (!g_var.envp[i])
		return (ERROR);
	free(g_var.envp[i]);
	j = i + 1;
	while (g_var.envp[j])
		j++;
	g_var.envp[i] = g_var.envp[--j];
	g_var.envp[j] = NULL;
	tmp = array_copy(g_var.envp, j);
	if (!tmp)
		return (ERROR);
	free_envp();
	g_var.envp = tmp;
	return (SUCCESS);
}
