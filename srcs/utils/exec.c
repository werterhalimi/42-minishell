/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shalimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 22:04:11 by shalimi           #+#    #+#             */
/*   Updated: 2022/12/09 22:20:47 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**env_to_paths(char **env)
{
	return (ft_split(&(env[ft_find_paths_index(env)][5]), ':'));
}

int	ft_find_paths_index(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_strncmp("PATH=", str[i], 5) == 0)
			return (i);
		i++;
	}
	return (-1);
}

char	*get_path(char **path, char *command)
{
	int		i;
	char	*ret;

	i = 0;
	while (path[i])
	{
		ret = ft_strjoin(path[i], ft_strjoin("/", command));
		if (!access(ret, X_OK))
			return (ret);
		i++;
	}
	ft_putstr_fd(command, 1);
	ft_putendl_fd(" n'existe pas.", 1);
	return (0);
}
