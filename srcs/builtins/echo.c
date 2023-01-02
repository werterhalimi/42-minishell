/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 10:23:11 by ncotte            #+#    #+#             */
/*   Updated: 2022/12/20 10:23:14 by ncotte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// \brief Check if the argument if of format: -n...n
/// \param arg the argument
/// \return 1 if it is, 0 otherwise
static int	valid_option(char const *arg)
{
	int	i;

	if (arg[0] != '-')
		return (NO);
	i = 0;
	while (arg[++i])
		if (arg[i] != 'n')
			break ;
	return (arg[i] == 0 && i > 1);
}

int	echo(char *argv[])
{
	int	i;
	int	n_option;

	i = 1;
	n_option = NO;
	if (argv[i])
	{
		while (argv[i] && valid_option(argv[i]) && i++)
			n_option = YES;
		while (argv[i])
		{
			ft_putstr_fd(argv[i], STDOUT_FILENO);
			if (argv[i + 1] && argv[i][0])
				write(STDOUT_FILENO, " ", 1);
			i++;
		}
	}
	if (n_option == NO)
		write(STDOUT_FILENO, "\n", 1);
	return (SUCCESS);
}
