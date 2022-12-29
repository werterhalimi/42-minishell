/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_char.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 11:59:55 by ncotte            #+#    #+#             */
/*   Updated: 2022/12/29 11:59:58 by ncotte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	remove_char(char *str, char c, int index)
{
	int	i;

	if (index < 0)
	{
		i = 0;
		while (str[i] && str[i] != c)
			i++;
		if (!str[i])
			return (ERROR);
	}
	else
		i = index;
	while (str[i])
	{
		str[i] = str[i + 1];
		i++;
	}
	return (SUCCESS);
}
