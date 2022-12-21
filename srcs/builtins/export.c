/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 16:34:05 by ncotte            #+#    #+#             */
/*   Updated: 2022/12/08 17:35:45 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_var(char *str)
{
	int	i;

	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	write(STDOUT_FILENO, str, i);
	if (str[i++] == '=')
	{
		write(STDOUT_FILENO, "=", 1);
		write(STDOUT_FILENO, "\"", 1);
		ft_putstr_fd(str + i, STDOUT_FILENO);
		write(STDOUT_FILENO, "\"", 1);
	}
	write(STDOUT_FILENO, "\n", 1);
}

static int	export_no_args(void)
{
	char	**envp;
	int		i;
	long	size;

	size = 0;
	while (g_var.envp[size])
		size++;
	envp = malloc(sizeof (*envp) * (size + 1));
	if (!envp)
		return (ERROR);
	i = 0;
	while (i < size)
	{
		envp[i] = g_var.envp[i];
		i++;
	}
	sort(envp, size);
	i = -1;
	while (envp[++i])
		if (ft_strncmp(envp[i], "_=", 2))
			print_var(envp[i]);
	free(envp);
	return (SUCCESS);
}

static int	add_variable(char *str, int size)
{
	char	**tmp;
	int		i;

	tmp = malloc(sizeof (*tmp) * (size + 2));
	if (!tmp)
		return (ERROR);
	i = -1;
	while (g_var.envp[++i])
		tmp[i] = g_var.envp[i];
	tmp[i] = ft_strdup(str);
	if (!tmp[i])
		return (ERROR);
	tmp[++i] = NULL;
	free(g_var.envp);
	g_var.envp = tmp;
	return (SUCCESS);
}

int	export(char *str)
{
	int		i;
	int		j;

	if (!str)
		return (export_no_args());
	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	j = 0;
	while (g_var.envp[j] && (ft_strncmp(g_var.envp[j], str, i) \
		|| (g_var.envp[j][i] && g_var.envp[j][i] != '=')))
		j++;
	if (g_var.envp[j] && str[i] == '=')
	{
		free(g_var.envp[j]);
		g_var.envp[j] = ft_strdup(str);
		if (!g_var.envp[j])
			return (ERROR);
	}
	else if (!g_var.envp[j])
		return (add_variable(str, j));
	return (SUCCESS);
}
