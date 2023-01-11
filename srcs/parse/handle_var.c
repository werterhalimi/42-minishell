/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shalimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 21:39:04 by shalimi           #+#    #+#             */
/*   Updated: 2023/01/11 19:03:00 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_len(char **str, int index, char *to_replace, char *new)
{
	int	len;

	len = ft_strlen(*str);
	if (!ft_strncmp(*str + index, to_replace, ft_strlen(to_replace)) \
				&& !is_between_single_quote(*str + index, index))
	{
		len -= ft_strlen(to_replace);
		len += ft_strlen(new);
	}
	return (len);
}

static int	str_replace_once(char **str, char *to_replace, char *new, int index)
{
	char	*tmp;
	int		ints[3];

	if (!new)
		new = "";
	ints[1] = get_len(str, index, to_replace, new);
	tmp = ft_calloc(sizeof(*tmp), ints[1] + 1);
	ints[0] = 0;
	ints[2] = 0;
	while ((*str)[ints[0]])
	{
		if (ints[0] == index && \
				!is_between_single_quote(str[0] + ints[0], ints[0]))
		{
			ft_memcpy(tmp + ints[2], new, ft_strlen(new));
			ints[2] += ft_strlen(new);
			ints[0] += ft_strlen(to_replace);
			continue ;
		}
		tmp[ints[2]++] = (*str)[ints[0]++];
	}
	tmp[ints[1]] = 0;
	free(*str);
	*str = tmp;
	return (index + (int) ft_strlen(new));
}

static char	*handle_last_er(char **line, int index, char **tmp)
{
	int	j;

	j = index;
	tmp[0] = ft_strdup((*line) + j++);
	if (tmp[0])
	{
		tmp[0][j - index] = 0;
		return (ft_itoa(g_var.last_er));
	}
	return (NULL);
}

static char	*handle_env_var(char **line, int index, char **tmp)
{
	int	j;

	j = 0;
	while (valid_var_name((*line)[j + index], j))
		j++;
	tmp[0] = ft_strdup((*line) + index);
	if (tmp[0])
	{
		tmp[0][j] = 0;
		tmp[1] = var_value(tmp[0]);
		if (tmp[1])
			return (ft_strdup(tmp[1]));
	}
	return (NULL);
}

void	handle_var(char **line)
{
	int		i;
	char	*value;
	char	*tmp[2];

	i = 0;
	while ((*line)[i])
	{
		if ((*line)[i++] == '$' && (*line)[i] && (*line)[i] != ' ')
		{
			if ((*line)[i] == '?')
				value = handle_last_er(line, i, tmp);
			else
				value = handle_env_var(line, i, tmp);
			if (tmp[0])
			{
				tmp[1] = ft_strjoin("$", tmp[0]);
				free(tmp[0]);
				if (tmp[1])
					i = str_replace_once(line, tmp[1], value, i - 1);
				free(tmp[1]);
				free(value);
			}
		}
	}
}
