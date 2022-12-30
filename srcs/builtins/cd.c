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

static int	update_env(char *new_dir)
{
	char	*str;
	int		ret;

	str = var_value("PWD");
	if (!str)
		return (ERROR);
	str = ft_strjoin("OLDPWD=", str);
	if (!str)
		return (ERROR);
	ret = export(str);
	free(str);
	if (ret)
		return (ERROR);
	str = ft_strjoin("PWD=", new_dir);
	if (!str)
		return (ERROR);
	ret = export(str);
	free(str);
	return (ret);
}

int	cd(char *argv[])
{
	char	*new_dir;
	int		ret;

	if (!argv[1])
	{
		new_dir = var_value("HOME");
		if (!new_dir || !new_dir[0])
			return (print_custom_error("minishell : cd: HOME not set"));
	}
	else if (!ft_strncmp(argv[1], "-", 2))
	{
		new_dir = var_value("OLDPWD");
		if (!new_dir || !new_dir[0])
			return (print_custom_error("minishell : cd: OLDPWD not set"));
	}
	else
		new_dir = argv[1];
	if (chdir(new_dir))
		return (ERROR && print_errno("minishell : cd", new_dir, errno));
	new_dir = getcwd(NULL, 0);
	ret = update_env(new_dir);
	if (argv[1] && !ft_strncmp(argv[1], "-", 2))
		ft_putendl_fd(new_dir, STDOUT_FILENO);
	free(new_dir);
	return (ret);
}
