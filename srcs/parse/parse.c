/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shalimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 20:11:45 by shalimi           #+#    #+#             */
/*   Updated: 2022/12/22 00:35:10 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_input(char *line, int *fd);

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
char	*handle_fd(int *in, int *out, char *line)
{
	int	len;
	int	input;
	int	output;

	input = 0;
	output = 0;
	len = ft_strlen(line);
	while (len >= 0 && (!input || !output))
	{
		if (line[len] == '<' && !input)
		{
			handle_input(&line[len - 1], in);
			input = len;
		}
		if (line[len] == '>' && !output)
		{
			(void) out;
			output = len;
		}
		len--;
	}
	return (ft_split(line, '<')[0]);
}

char	*get_string(char **split, char *current, int *index, int len)
{
	int	i;

	if (!index)
		i = 0;
	else
		i = *index;
	if (ft_strlen(current) == 1 && i < len - 1)
		current = ft_strjoin(current, split[++i]);
	if (current[0] == '\'' || current[0] == '"')
		while (current[ft_strlen(current) - 1] != current[0] && i < len)
		{
			current = join(current,split[++i]);
			free(split[i]);
		}
	if (current[0] != '"' && current[0] != '\'' && !index)
	{
		split = ft_split(current, ' ');
		current = split[0];
		free(split);
	}
	current = ft_baskslash(current);
	if (current[0] == current[ft_strlen(current) - 1] && (
				current[0] == '\'' ||
				current[0] == '"'))
	{
		current = ft_substr(current, 1, ft_strlen(current) - 2);
	}
	if (index)
		*index = i;
	return (current);
}



void	handle_input(char *line, int *fd)
{
	int	f[2];
	char	*tmp;
	char	*buff;
	char	*sep;
	char	**split;

	tmp = line;
	if (line[1] == '<')
	{
		line = &line[2];
		while (*line == ' ') line++;
		split = ft_split(line, ' ');
		line = ft_strtrim(line, " 	");
		sep = get_string(split, line, 0, ft_strlen(line));
		f[0] = 0;
		while (f[0] < (int) ft_strlen(tmp))
		{
			tmp[f[0]] = ' ';
			f[0]++;
		}
		buff = "";
		pipe(f);
		while (1)
		{
			printf("%s", sep);
			buff = readline(">");
			if (!ft_strncmp(buff, sep, ft_strlen(buff)) && ft_strlen(buff) == ft_strlen(sep))
				break ;
			write(f[1], buff, ft_strlen(buff));
			write(f[1], "\n", 1);
		}
		close(f[1]);
		fd[0] = f[0];
		free(split);
		free(line);
	}
	else
	{
		line = &line[1];
		while (*line == ' ') line++;
		split = ft_split(line, ' ');
		line = ft_strtrim(line, "	 ");
		fd[0] = open(get_string(split, line, 0, ft_strlen(line)), O_RDONLY);
		f[0] = 0;
		while (f[0] < (int) ft_strlen(tmp))
		{
			tmp[f[0]] = ' ';
			f[0]++;
		}
		free(line);
		free(split);
	}
}

void	handle_output(char *line, int *fd)
{
	int		append;
	char	**split;
	int		i;
	char	*tmp;

	tmp = line;
	i = 0;
	append = 0;
	if (line[1] == '>')
	{
		append = 1;
		line += 1;
	}
	line = &line[1];
	while (*line == ' ') line++;
	split = ft_split(line, ' ');
	line = ft_strtrim(line, " 	");
	if (!append)
		fd[1] = open(get_string(split, line, 0, ft_strlen(line)), O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd[1] = open(get_string(split, line, 0, ft_strlen(line)), O_WRONLY | O_CREAT |  O_APPEND, 0644);
	while (i < (int) ft_strlen(tmp))
	{
		tmp[i] = ' ';
		i++;
	}
	free(line);
	free(split);
}

void	handle_line(char *line, t_command *cmd, int fd[2])
{
	int		len;
	char	c;

	len = ft_strlen(line);
	while (len >= 0)
	{
		c = line[len];
		if (c == '<')
		{
			if (line[len - 1] == '<')
				handle_input(&line[--len], fd);
			else
				handle_input(&line[len], fd);
		}
		if (c == '>')
		{
			if (line[len - 1] == '>')
				handle_output(&line[--len], fd);
			else
				handle_output(&line[len], fd);
		}
		len--;
	}
	cmd->fd[0] = fd[0];
	cmd->fd[1] = fd[1];
}

void	remove_quote(char *str)
{
	int	i;
	int	no_double;
	int	no_single;
	int	final_len;

	i = 0;
	no_double = 0;
	no_single = 0;
	while (str[i])
	{
		if (str[i] == '\'' && no_double % 2 == 0)
			no_single++;
		if (str[i] == '"' && no_single % 2 == 0)
			no_double++;
		i++;
	}
	no_double -= no_double % 2;
	no_single -= no_single % 2;
	final_len = ft_strlen(str) - no_double - no_single;
	i = 0;
	while (str[i])
	{
		if (str[i] == '"')
		{
			if (no_double == 0)
			{
				i++;
				continue;
			}
			no_double--;
			ft_memmove(str + i, str+ 1 + i, ft_strlen(str + 1 + i));
			str[ft_strlen(str) - 1] = 0;
			continue ;
		}
		if (str[i] == '\'')
		{
			if (no_single == 0)
			{
				i++;
				continue;
			}
			no_single--;
			ft_memmove(str +i, str + 1 + i, ft_strlen(str + 1 + i));
			str[ft_strlen(str) - 1] = 0;
			continue ;
		}
		i++;
	}
	str[final_len] = 0;
}

void	handle_quote(char **split, int len)
{
	int	i;

	remove_quote(split[0]);
	i = 1;
	while  (i < len)
	{
		remove_quote(split[i]);
		i++;
	}
}

t_command	parse(char *line, int fd[2])
{
	char		**split;
	int			len;
	char		*current;
	t_command	ret;
	int			i;
	int			y;

	handle_line(line, &ret, fd);
	line = ft_strtrim(line, "	 ");
	len = ft_countchar(line, ' ') + 1;
	split = ft_split(line, ' ');
	handle_quote(split, len);
	ret.args = ft_alloc(sizeof(*(ret.args)), len, g_var.parse_alloc);
	i = 0;
	y = 0;
	while (i < len)
	{
		current = ft_strtrim(split[i], " ");
		current = get_string(split, current, &i, len);
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
	free(split);
	return (ret);
}
