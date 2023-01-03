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

/// \brief Move all environment variable into a new array
/// \param nb_variables the number of variables
/// \return 0 on SUCCESS, 1 if ERROR
static int	rearrange_env(int nb_variables)
{
	int		i;
	char	**tmp;

	tmp = malloc(sizeof (*tmp) * (nb_variables + 1));
	if (!tmp)
		return (ERROR);
	i = -1;
	while (++i < nb_variables)
		tmp[i] = g_var.envp[i];
	tmp[i] = NULL;
	free(g_var.envp);
	g_var.envp = tmp;
	return (SUCCESS);
}

int	unset_one_var(char *name, int rearrange)
{
	int		i;
	int		j;
	size_t	length;

	i = 0;
	length = ft_strlen(name);
	while (g_var.envp[i] && (ft_strncmp(g_var.envp[i], name, length) \
		|| (g_var.envp[i][length] && g_var.envp[i][length] != '=')))
		i++;
	if (!g_var.envp[i])
		return (SUCCESS);
	free(g_var.envp[i]);
	j = i + 1;
	while (g_var.envp[j])
		j++;
	g_var.envp[i] = g_var.envp[--j];
	g_var.envp[j] = NULL;
	if (rearrange == NO)
		return (SUCCESS);
	return (rearrange_env(j));
}

int	unset(char *argv[])
{
	int	i;

	if (!argv[1])
		return (SUCCESS);
	i = 1;
	while (argv[i])
		unset_one_var(argv[i++], NO);
	i = 0;
	while (g_var.envp[i])
		i++;
	return (rearrange_env(i));
}
