/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shalimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 20:11:45 by shalimi           #+#    #+#             */
/*   Updated: 2022/12/15 16:55:54 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_countchar(const char *s, char c)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	if (c == 0)
		return (-1);
	while (s[i])
	{
		if (s[i++] == c)
		{
			if (s[i] == c || !s[i])
				continue ;
			else
				ret++;
		}
	}
	if (!ret)
		return (-1);
	if (s[0] == c || s[i - 1] == c)
		ret--;
	return (ret);
}

char	*ft_baskslash(char *str)
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

char	*join(char *s1, char *s2)
{
	char	*buff;
	char	*ret;

	buff = ft_strjoin(s1, " ");
	ret = ft_strjoin(buff, s2);
	return (ret);
}

t_command	parse(char *line)
{
	char		**split;
	int			len;
	char		*current;
	t_command	ret;
	int			i;
	int			y;

	len = ft_countchar(ft_strtrim(line, " "), ' ') + 1;
	split = ft_split(line, ' ');
	ret.args = ft_alloc(sizeof(*(ret.args)), len, g_var.parse_alloc);
	i = 0;
	y = 0;
	while (i < len)
	{
		current = ft_strtrim(split[i], " ");
		if (current[0] == '\'' || current[0] == '"')
			while (current[ft_strlen(current) - 1] != current[0] && i < len)
				current = join(current,split[++i]);
		current = ft_baskslash(current);
		if (current[0] == current[ft_strlen(current) - 1] && (
					current[0] == '\'' ||
					current[0] == '"'))
			{
				current = ft_substr(current, 1, ft_strlen(current) - 2);
			}
		ret.args[y++] = current;
		i++;
	}
	if (len != 0)
		ret.args[y] = 0;
	else
	{
		ret.args[0] = line;
		ret.args[1] = 0;
	}
	ret.command = split[0];
	ret.argc = y;
	return (ret);
}
