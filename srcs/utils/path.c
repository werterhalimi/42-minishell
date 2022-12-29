/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shalimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 22:04:11 by shalimi           #+#    #+#             */
/*   Updated: 2022/12/28 18:41:45 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**env_to_paths(void)
{
	char	*paths_line;
	char	**paths;

	paths = NULL;
	paths_line = var_value("PATH");
	if (paths_line)
		paths = ft_split(paths_line, ':');
	return (paths);
}

char	*get_path(char **path, char *command)
{
	int		i;
	char	*ret;

	if (path)
	{
		i = 0;
		while (path[i])
		{
			ret = ft_strjoin(path[i], ft_strjoin("/", command));
			if (!access(ret, X_OK))
				return (ret);
			i++;
		}
		if (!access(command, X_OK))
			return (command);
	}
	ft_putstr_fd(command, STDERR_FILENO); // TODO
	ft_putendl_fd(" n'existe pas.", STDERR_FILENO);
	return (NULL);
}
