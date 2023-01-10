/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shalimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 20:11:45 by shalimi           #+#    #+#             */
/*   Updated: 2023/01/04 21:47:15 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_countchar(const char *s, char c)
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
	if (s[0] == c || s[i - 1] == c)
		ret--;
	return (ret);
}

static void	str_builder(char **str, char to_append)
{
	char	*tmp;
	int		len;

	len = ft_strlen(*str);
	tmp = ft_calloc(sizeof(*tmp), len + 1);
	ft_memcpy(tmp, *str, len);
	tmp[len] = to_append;
	free(*str);
	*str = ft_calloc(sizeof(*tmp), len + 2);
	ft_memcpy(*str, tmp, len + 1);
	free(tmp);
}

static void	parse_line(char **args, char *line, int len)
{
	int	i;
	int	words;

	if (len == 0)
	{
		free(args);
		args = ft_calloc(sizeof(*args), 2);
		(args)[0] = line;
		return ;
	}
	i = 0;
	words = 0;
	while (i < (int) ft_strlen(line))
	{
		if ((line[i] == ' ' || line[i] == '\t')
			&& !is_between_quote(&line[i], i))
		{
			if (++i && !(line[i - 2] == ' ' || line[i - 2] == '\t'))
				words++;
			continue ;
		}
		str_builder(args + words, line[i++]);
	}
	(args)[words + 1] = NULL;
}

// gerer cas "cat << >>" ou "cat << |"
t_command	parse(char *line, int fd[2])
{
	t_command	ret;
	int			i;
	char		*tmp;

	ret.parse_error = 0;
	handle_tilde(&line);
	handle_var(&line);
	handle_line(line, &ret, fd);
	tmp = line;
	line = ft_strtrim(line, "\t ");
	free(tmp);
	ret.len = ft_countchar(line, ' ') + 1;
	ret.args = ft_calloc(sizeof(*(ret.args)), ret.len + 1);
	i = 0;
	while (i < ret.len)
		ret.args[i++] = ft_strdup("");
	ret.args[i] = NULL;
	parse_line(ret.args, line, ret.len);
	handle_quote(ret.args, ret.len);
	ret.command = ret.args[0];
	free(line);
	return (ret);
}
