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

/// \brief Print the name of the variable
/// \n and its value between double quotes
/// \param str the name and value of the variable (name=value)
static void	print_var(char *str)
{
	int	i;

	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	write(STDOUT_FILENO, str, i);
	if (str[i++] == '=')
		ft_printf("=\"%s\"", str + i);
	write(STDOUT_FILENO, "\n", 1);
}

/// \brief export bash command without arguments
/// \n (print all declared environment variables except $_)
/// \return 0 on SUCCESS, 1 if ERROR
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
	while (i <= size)
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

int	export(char **argv)
{
	int	i;
	int	ret_value;

	if (!argv[1])
		return (export_no_args());
	i = 1;
	ret_value = SUCCESS;
	while (argv[i])
		if (export_one_var(argv[i++]))
			ret_value = ERROR;
	return (ret_value);
}
