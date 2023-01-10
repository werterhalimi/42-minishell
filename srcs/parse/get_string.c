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

static char	*join(char *s1, char *s2)
{
	char	*buff;
	char	*ret;

	buff = ft_strjoin(s1, " ");
	free(s1);
	ret = ft_strjoin(buff, s2);
	free(buff);
	free(s2);
	return (ret);
}

static char	*join_and_free(char *current, char *split)
{
	char	*tmp;

	tmp = current;
	current = ft_strjoin(current, split);
	free(tmp);
	return (current);
}

static char	*remove_str_space(char *current)
{
	int		i;

	i = 0;
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

static char	*ft_backslash(char *str)
{
	int	i;
	int	len;

	len = (int) ft_strlen(str);
	i = 0;
	while (i < len)
	{
		if (str[i] == '\\')
		{
			ft_memcpy(str + i, str + i + 1, ft_strlen(str + i));
			len -= 1;
			str[len] = 0;
		}
		i++;
	}
	return (str);
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
	current = ft_backslash(current);
	if (current[0] == current[ft_strlen(current) - 1] \
		&& (current[0] == '\'' || current[0] == '"'))
		current = ft_substr(current, 1, ft_strlen(current) - 2);
	return (current);
}
