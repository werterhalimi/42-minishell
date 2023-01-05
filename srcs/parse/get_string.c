/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_string.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shalimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 17:25:37 by shalimi           #+#    #+#             */
/*   Updated: 2023/01/05 22:31:01 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*join_and_free(char *current, char *split)
{
	char	*tmp;

	tmp = current;
	current = ft_strjoin(current, split);
	free(tmp);
	return (current);
}

char	*remove_str_space(char *current)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = current;
	while (current[i])
	{
		if (current[i] == ' ' && !is_between_quote(current + i, i))
		{
			current = ft_substr(current, 0, i);
		}
		i++;
	}
	return (current);
}

char	*get_string(char **split, char *current, int i, int len)
{
	if (current[0] == 0)
		return (0);
	current = remove_str_space(current);
	if (ft_strlen(current) == 1 && i < len - 1)
		current = join_and_free(current, split[++i]);
	if (current[0] == '\'' || current[0] == '"')
	{
		while (current[ft_strlen(current) - 1] != current[0] && i < len)
		{
			current = join(current, split[++i]);
			free(split[i]);
		}
	}
	current = ft_baskslash(current);
	if (current[0] == current[ft_strlen(current) - 1] \
		&& (current[0] == '\'' || current[0] == '"'))
		current = ft_substr(current, 1, ft_strlen(current) - 2);
	return (current);
}
