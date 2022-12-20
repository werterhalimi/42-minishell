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
	write(STDOUT_FILENO, str, ++i);
	write(STDOUT_FILENO, "\"", 1);
	ft_putstr_fd(str + i, STDOUT_FILENO);
	ft_putendl_fd("\"", STDOUT_FILENO);
}

static int	export_no_args(void)
{
	char	**envp;
	int		i;
	long	size;
	t_list	*tmp;

	size = ft_lstsize(g_var.envp);
	envp = malloc(sizeof (*envp) * (size + 1));
	if (!envp)
		return (ERROR);
	tmp = g_var.envp;
	i = 0;
	while (i < size)
	{
		envp[i++] = tmp->content;
		tmp = tmp->next;
	}
	sort(envp, size);
	i = -1;
	while (envp[++i])
		if (ft_strncmp(envp[i], "_=", 2))
			print_var(envp[i]);
	free(envp);
	return (SUCCESS);
}

int	export(char *str)
{
	int		i;
	t_list	*list;
	char	*tmp;

	if (!str)
		return (export_no_args());
	i = 0;
	list = g_var.envp;
	while (str[i] && str[i] != '=')
		i++;
	if (str[i])
	{
		while (list && list->next && ft_strncmp(list->content, str, i))
			list = list->next;
		if (list && list->next)
		{
			free(list->content);
			list->content = ft_strdup(str);
			if (!list->content)
				return (1);
		}
		else if (list && !list->next)
		{
			tmp = ft_strdup(str);
			if (!tmp)
				return (1);
			list->next = ft_lstnew(tmp);
			if (!list->next)
				return (1);
		}
		else
			return (1);
	}
	return (0);
}
