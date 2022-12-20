/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_copy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 17:33:35 by ncotte            #+#    #+#             */
/*   Updated: 2022/12/20 17:33:37 by ncotte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**array_copy(char *src[], int size)
{
	char	**copy;
	int		i;

	copy = malloc(sizeof (*copy) * (size + 1));
	if (copy)
	{
		i = 0;
		while (i < size)
		{
			copy[i] = ft_strdup(src[i]);
			if (!copy[i++])
			{
				while (--i >= 0)
					free(copy[i]);
				free(copy);
				return (NULL);
			}
		}
		copy[i] = NULL;
	}
	return (copy);
}
