/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 10:28:46 by ncotte            #+#    #+#             */
/*   Updated: 2022/12/20 10:28:48 by ncotte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	error_msg(char *str)
{
	ft_putendl_fd(str, STDERR_FILENO);
	return (ERROR);
}

static int	cd_error(char *argv[])
{
	ft_putstr_fd("cd: ", STDERR_FILENO);
	if (argv[2])
		ft_putstr_fd("string not in pwd: ", STDERR_FILENO);
	else
	{
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putstr_fd(argv[1], STDERR_FILENO);
	write(STDERR_FILENO, "\n", 1);
	return (ERROR);
}

static int	update_env(char *new_dir)
{
	char	*str;

	str = var_value("PWD");
	if (!str)
		return (ERROR);
	str = ft_strjoin("OLDPWD=", str);
	if (!str)
		return (ERROR);
	export(str);
	free(str);
	str = ft_strjoin("PWD=", new_dir);
	if (!str)
		return (ERROR);
	export(str);
	free(str);
	return (SUCCESS);
}

int	cd(char *argv[])
{
	char	*new_dir;

	if (!argv[1])
	{
		new_dir = var_value("HOME");
		if (!new_dir || !new_dir[0])
			return (error_msg("minishell : cd: HOME not set"));
	}
	else if (!ft_strncmp(argv[1], "-", 2))
	{
		new_dir = var_value("OLDPWD");
		if (!new_dir || !new_dir[0])
			return (error_msg("minishell : cd: OLDPWD not set"));
	}
	else
		new_dir = argv[1];
	if (chdir(new_dir))
		return (cd_error(argv));
	new_dir = getcwd(NULL, 0);
	update_env(new_dir);
	free(new_dir);
	return (SUCCESS);
}
