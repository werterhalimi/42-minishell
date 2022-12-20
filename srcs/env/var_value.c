/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_value.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 10:39:36 by ncotte            #+#    #+#             */
/*   Updated: 2022/12/20 10:39:39 by ncotte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*var_value(char const *var_name)
{
	size_t	length;
	int		i;

	i = 0;
	length = ft_strlen(var_name);
	while (g_var.envp[i] && ft_strncmp(g_var.envp[i], var_name, length))
		i++;
	if (g_var.envp[i])
		return (g_var.envp[i] + length + 1);
	return (NULL);
}
