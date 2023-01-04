/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shalimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 21:39:04 by shalimi           #+#    #+#             */
/*   Updated: 2023/01/04 21:42:01 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_var(char **line)
{
	int		i;
	int		j;
	char	*value;
	char	*tmp[2];

	i = 0;
	value = NULL;
	while((*line)[i])
	{
		if ((*line)[i++] == '$' && (*line)[i])
		{
			j = i;
			if ((*line)[j] == '?')
			{
				tmp[0] = ft_strdup((*line) + j++);
				if (tmp[0])
				{
					tmp[0][j - i] = 0;
					value = ft_itoa(g_var.last_er);
				}
			}
			else
			{
				while (ft_isalnum((*line)[j]))
					j++;
				tmp[0] = ft_strdup((*line) + i);
				if (tmp[0])
				{
					tmp[0][j - i] = 0;
					tmp[1] = var_value(tmp[0]);
					if (tmp[1])
						value = ft_strdup(tmp[1]);
				}
			}
			if (tmp[0])
			{
				tmp[1] = ft_strjoin("$", tmp[0]);
				free(tmp[0]);
				if (tmp[1])
				{
					i = str_replace_once(line, tmp[1], value, i - 1);
					free(tmp[1]);
					free(value);
				}
			}
		}
	}
}

