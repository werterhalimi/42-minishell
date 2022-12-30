/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shalimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 20:11:45 by shalimi           #+#    #+#             */
/*   Updated: 2022/12/29 21:53:05 by shalimi          ###   ########.fr       */
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
	if (s2)
	{
		ret = ft_strjoin(buff, s2);
		free(buff);
	}
	else
		ret = buff;
	return (ret);
}
/*
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
			handle_input(&line[len - 1], in, 0);
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
}*/

char	*get_string(char **split, char *current, int *index, int len)
{
	int	i;

	if (current[0] == 0)
		return (0);
	if (!index)
		i = 0;
	else
		i = *index;
	if (ft_strlen(current) == 1 && i < len - 1) // TODO ????
		current = ft_strjoin(current, split[++i]);
	if (current[0] == '\'' || current[0] == '"')
	{
		while (current[ft_strlen(current) - 1] != current[0] && i < len) {
			current = join(current, split[++i]);
			free(split[i]);
		}
	}
	if (current[0] != '"' && current[0] != '\'' && !index)
	{
		split = ft_split(current, ' ');
		current = split[0];
		free(split);
	}
	current = ft_baskslash(current);
	if (current[0] == current[ft_strlen(current) - 1] \
		&& (current[0] == '\'' || current[0] == '"'))
		current = ft_substr(current, 1, ft_strlen(current) - 2);
	if (index)
		*index = i;
	return (current);
}


//void	str_replace(char **str, char *to_replace, char *new);

void	set_fd(int *fd, int value)
{
	if (*fd != STDIN_FILENO && *fd != STDOUT_FILENO && *fd != STDERR_FILENO)
		return ;
	*fd = value;
}

void	handle_input(char *line, int *fd, t_command *cmd)
{
	int		f[2];
	char	*tmp;
	char	*buff;
	char	*sep;
	char	**split;

	tmp = line;
	line = ft_strtrim(line, "	 ");
	if (ft_strlen(line) < 2 || (ft_strlen(line) == 2 && line[0] == line[1]))
	{
		cmd->parse_error = SYNTAX_ERROR * (-1);
		return ;
	}
	if (line[1] == '<')
	{
		line = &line[2];
		while (*line == ' ')
			line++;
		split = ft_split(line, ' ');
		line = ft_strtrim(line, " 	");
		sep = get_string(split, line, 0, ft_strlen(line));
		if (!sep)
			return;
		f[0] = 0;
		while (f[0] < (int) ft_strlen(tmp))
		{
			tmp[f[0]] = ' ';
			f[0]++;
		}
		buff = "";
		pipe(f);
		g_var.status = HEREDOC;
		g_var.pid = fork();
		if (g_var.pid == 0)
		{
			while (1)
			{
				signals();
				printf("%s", sep);
				buff = readline(">");
				if (!buff || (!ft_strncmp(buff, sep, ft_strlen(buff)) && ft_strlen(buff) == ft_strlen(sep)))
					exit(SUCCESS);
				write(f[1], buff, ft_strlen(buff));
				write(f[1], "\n", 1);
			}
		}
		signals();
		g_var.status = EXECUTE;
		close(f[1]);
		set_fd(&fd[0], f[0]);

		free(split);
		free(line);
	}
	else
	{
		line = &line[1];
		while (*line == ' ')
			line++;
		split = ft_split(line, ' ');
		line = ft_strtrim(line, "	 ");
		set_fd(&fd[0],open(get_string(split, line, 0, ft_strlen(line)), O_RDONLY));
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

void	handle_output(char *line, int *fd, t_command *cmd)
{
	int		append;
	char	**split;
	int		i;
	char	*tmp;

	tmp = line;
	i = 0;
	append = 0;
	line = ft_strtrim(line, "	 ");
	if (ft_strlen(line) < 2 || (ft_strlen(line) == 2 && line[0] == line[1]))
	{
		cmd->parse_error = SYNTAX_ERROR * (-1);
		return ;
	}
	if (line[1] == '>')
	{
		append = 1;
		line += 1;
	}
	line = &line[1];
	while (*line == ' ')
		line++;
	split = ft_split(line, ' ');
	line = ft_strtrim(line, " 	");
	if (!append)
		set_fd(&fd[1], open(get_string(split, line, 0, ft_strlen(line)), O_WRONLY | O_CREAT | O_TRUNC, 0644));
	else
		set_fd(&fd[1], open(get_string(split, line, 0, ft_strlen(line)), O_WRONLY | O_CREAT | O_APPEND, 0644));
	while (i < (int) ft_strlen(tmp))
	{
		tmp[i] = ' ';
		i++;
	}
	free(line);
	free(split);
}

int	is_between_quote(char *word, int index)
{
	int	right;
	int	left;
	int	i;

	right = 0;
	left = 0;
	i = 1;
	while (i <= index)
	{
		if (*(word - i) == '\'' || *(word - i) == '"')
			left++;
		i++;
	}
	if (word[0])
	{
		i = 1;
		while (word[i])
		{
			if (word[i] == '"' || word[i] == '\'')
				right++;
			i++;
		}
	}
	return (right % 2 == 1 && left % 2 == 1);
}

void	handle_line(char *line, t_command *cmd, int fd[2])
{
	int		len;
	char	c;

	len = ft_strlen(line);
	while (len >= 0)
	{
		if (is_between_quote(line + len, len))
		{
			len--;
			continue ;
		}
		c = line[len];
		if (c == '<' && !is_between_quote(line + len, len))
		{
			if (line[len - 1] == '<')
				handle_input(&line[--len], fd, cmd);
			else
				handle_input(&line[len], fd, cmd);
		}
		if (c == '>')
		{
			if (line[len - 1] == '>')
				handle_output(&line[--len], fd, cmd);
			else
				handle_output(&line[len], fd, cmd);
		}
		len--;
	}
	cmd->fd[0] = fd[0];
	cmd->fd[1] = fd[1];
	if (cmd->parse_error == SYNTAX_ERROR * -1) // TODO
		ft_putendl_fd("syntax error near unexpected token 'newline'", STDERR_FILENO);
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
				continue ;
			}
			no_double--;
			ft_memmove(str + i, str + 1 + i, ft_strlen(str + 1 + i));
			str[ft_strlen(str) - 1] = 0;
			continue ;
		}
		if (str[i] == '\'')
		{
			if (no_single == 0)
			{
				i++;
				continue ;
			}
			no_single--;
			ft_memmove(str + i, str + 1 + i, ft_strlen(str + 1 + i));
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

	if (!split[0])
		return ; 
	remove_quote(split[0]);
	i = 1;
	while  (i < len)
	{
		remove_quote(split[i]);
		i++;
	}
}


int	is_between_single_quote(char *word, int index)
{
	int	right;
	int	left;
	int	i;

	right = 0;
	left = 0;
	i = 1;
	while (i <= index)
	{
		if (*(word - i) == '\'')
			left++;
		i++;
	}
	if (word[0])
	{
		i = 1;
		while (word[i])
		{
			if (word[i] == '\'')
				right++;
			i++;
		}
	}
	return (right % 2 == 1 && left % 2 == 1);
}




void	str_replace(char **str, char *to_replace, char *new)
{
	char	*tmp;
	int		i;
	int		len;
	int		no;

	i = 0;
	len = ft_strlen(*str);
	if (!new)
		new = "";
	while (i < (int) ft_strlen(*str))
	{
		if (ft_strncmp(*str + i, to_replace, ft_strlen(to_replace)) == 0)
		{
			len -= ft_strlen(to_replace);
			len += ft_strlen(new);
		}
		i++;
	}
	tmp = ft_calloc(sizeof(*tmp), len + 1);
	i = 0;
	no = 0;
	while (i < len)
	{
		if (ft_strncmp((*str) + i, to_replace, ft_strlen(to_replace)) == 0
				&& !is_between_single_quote(str[0] + no, no))
		{
			ft_memcpy(tmp + i, new, ft_strlen(new) );
			i += ft_strlen(new);
			no += ft_strlen(to_replace);
			continue ;
		}
		tmp[i] = (*str)[no];
		i++;
		no++;
	}
	tmp[len] = 0;
//	free(*str);
	*str = tmp;
}

void	handle_var(char **line)
{
	char	*tmp;
	char	*tmp2;
	int		i;
	int		j;

	i = 0;
	while (g_var.envp[i])
	{
		j = 0;
		while (g_var.envp[i][j] && g_var.envp[i][j] != '=')
			j++;
		tmp = ft_strdup(g_var.envp[i]);
		tmp[j] = 0;
		tmp2 = ft_strjoin("$", tmp);
		str_replace(line, tmp2, var_value(tmp));
		free(tmp);
		free(tmp2);
		i++;
	}
	str_replace(line, "$?", ft_itoa(g_var.last_er));
}

void	handle_tilde(char **line)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (line[0][i])
	{
		if (line[0][i] == '~')
		{
			tmp = ft_strjoin(var_value("HOME"),line[0] + i + 1);
			j = 0;
			while (tmp[j])
			{
				if (tmp[j] == ' ')
				{
					tmp[j] = 0;
					break ;
				}
				j++;
			}
			if (!access(tmp, F_OK))
				str_replace(line, "~", var_value("HOME"));
			free(tmp);
		}
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

	ret.parse_error = 0;
	// TODO replace var
	handle_tilde(&line);
	handle_var(&line);
	handle_line(line, &ret, fd);
	line = ft_strtrim(line, "	 ");
	len = ft_countchar(line, ' ') + 1;
	split = ft_split(line, ' ');
	handle_quote(split, len);
	ret.args = ft_alloc(sizeof(*(ret.args)), len + 1, g_var.parse_alloc);
	i = 0;
	y = 0;
	while (i < len && split[i])
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
		ret.args = ft_alloc(sizeof(*(ret.args)), 2, g_var.parse_alloc); // TODO
		ret.args[0] = line;
		ret.args[1] = 0;
	}
	ret.command = split[0];
	free(split);
	return (ret);
}
