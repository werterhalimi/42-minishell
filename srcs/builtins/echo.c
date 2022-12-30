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

int	echo(char *argv[])
{
	int	i;
	int	n_option;

	i = 1;
	n_option = 0;
	if (argv[i])
	{
		while (argv[i] && !ft_strncmp(argv[i], "-n", 2) && i++)
			n_option = 1;
		while (argv[i])
		{
			ft_putstr_fd(argv[i], STDOUT_FILENO);
			if (argv[i + 1] && argv[i][0])
				write(STDOUT_FILENO, " ", 1);
			i++;
		}
	}
	if (!n_option)
		write(STDOUT_FILENO, "\n", 1);
	return (SUCCESS);
}
