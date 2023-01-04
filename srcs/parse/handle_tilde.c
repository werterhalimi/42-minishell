/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_tilde.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shalimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 18:34:37 by shalimi           #+#    #+#             */
/*   Updated: 2023/01/04 18:46:38 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_space(char *tmp)
{
	int	j;

	j = 0;
	while (tmp[j])
	{
		if (tmp[j] == ' ' || tmp[j] == '\t')
		{
			tmp[j] = 0;
			break ;
		}
		j++;
	}
}

void	handle_tilde(char **line)
{
	int		i;
	char	*tmp;
	char	*tmp2;

	i = 0;
	tmp2 = *line;
	while (line[0][i])
	{
		if (line[0][i] == '~')
		{
			tmp = ft_strjoin(var_value("HOME"), line[0] + i + 1);
			remove_space(tmp);
			if (!access(tmp, F_OK))
			{
				str_replace(line, "~", var_value("HOME"));
				free(tmp2);
			}
			free(tmp);
		}
		i++;
	}
}
