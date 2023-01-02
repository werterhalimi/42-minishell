/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shalimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 20:11:45 by shalimi           #+#    #+#             */
/*   Updated: 2022/12/30 19:54:38 by shalimi          ###   ########.fr       */
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
	free(s1);
	ret = ft_strjoin(buff, s2);
	free(buff);
	free(s2);
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
	int		i;
	char	*tmp;

	if (current[0] == 0)
		return (0);
	if (!index)
		i = 0;
	else
		i = *index;
	if (ft_strlen(current) == 1 && i < len - 1)
	{
		tmp = current;
		current = ft_strjoin(current, split[++i]);
		free(tmp);
	}
	if (current[0] == '\'' || current[0] == '"')
	{
		while (current[ft_strlen(current) - 1] != current[0] && i < len) {
			current = join(current, split[++i]);
			free(split[i]);
		}
	}
	if (current[0] != '"' && current[0] != '\'' && !index)
	{
		split = ft_split(current, ' '); // TODO \t ?
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
	line = ft_strtrim(line, "\t ");
	if (ft_strlen(line) < 2 || (ft_strlen(line) == 2 && line[0] == line[1]))
	{
		cmd->parse_error = SYNTAX_ERROR * (-1);
		return ;
	}
	if (line[1] == '<')
	{
		line = &line[2];
		while (*line == ' ') // TODO \t ?
			line++;
		split = ft_split(line, ' '); // TODO \t ?
		line = ft_strtrim(line, "\t ");
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
		while (*line == ' ') // TODO \t ?
			line++;
		split = ft_split(line, ' '); // TODO \t ?
		line = ft_strtrim(line, "\t ");
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
	line = ft_strtrim(line, "\t ");
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
	while (*line == ' ') // TODO \t ?
		line++;
	split = ft_split(line, ' '); // TODO \t ?
	line = ft_strtrim(line, "\t ");
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
		if (c == '<')
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

	if (split[0])
		remove_quote(split[0]);
	i = 1;
	while  (i < len)
	{
		if (split[i])
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
	while ((*str)[i]) // (i < (int) ft_strlen(*str))
	{
		if (ft_strncmp(*str + i++, to_replace, ft_strlen(to_replace)) == 0)
		{
			len -= ft_strlen(to_replace);
			len += ft_strlen(new);
		}
//		i++;
	}
	tmp = ft_calloc(sizeof(*tmp), len + 1);
	i = 0;
	no = 0;
	while ((*str)[i])
	{
		if (ft_strncmp(*str + i, to_replace, ft_strlen(to_replace)) == 0
			&& !is_between_single_quote(str[0] + i, i))
		{
			ft_memcpy(tmp + no, new, ft_strlen(new));
			no += ft_strlen(new);
			i += ft_strlen(to_replace);
			continue ;
		}
		tmp[no++] = (*str)[i++];
	}
//	while (i < len)
//	{
//		if (ft_strncmp(*str + i, to_replace, ft_strlen(to_replace)) == 0
//				&& !is_between_single_quote(str[0] + no, no))
//		{
//			ft_memcpy(tmp + i, new, ft_strlen(new) );
//			i += ft_strlen(new);
//			no += ft_strlen(to_replace);
//			continue ;
//		}
//		tmp[i] = (*str)[no];
//		i++;
//		no++;
//	}
	tmp[len] = 0;
//	free(*str); // TODO ?
	*str = tmp;
}

int	str_replace_once(char **str, char *to_replace, char *new, int index)
{
	char	*tmp;
	int		i;
	int		len;
	int		no;

	len = ft_strlen(*str);
	if (!new)
		new = "";
	if (! ft_strncmp(*str + index, to_replace, ft_strlen(to_replace))) // TODO ??
	{
		len -= ft_strlen(to_replace);
		len += ft_strlen(new);
	}
	tmp = ft_calloc(sizeof(*tmp), len + 1);
	i = 0;
	no = 0;
	while ((*str)[i])
	{
		if (i == index && !is_between_single_quote(str[0] + i, i))
		{
			ft_memcpy(tmp + no, new, ft_strlen(new));
			no += ft_strlen(new);
			i += ft_strlen(to_replace);
			continue ;
		}
		tmp[no++] = (*str)[i++];
	}
	tmp[len] = 0;
	//	free(*str); // TODO ?
	*str = tmp;
	return (index + (int) ft_strlen(new));
}

void	handle_var(char **line) // TODO echo $$ ?
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
/*
void	handle_var(char **line) // TODO non-existing var & too long
{
	char	*tmp[3];
	int		i;
	int		j;

	i = 0;
	while (g_var.envp[i])
	{
		j = 0;
		while (g_var.envp[i][j] && g_var.envp[i][j] != '=')
			j++;
		tmp[0] = ft_strdup(g_var.envp[i++]);
		tmp[0][j] = 0;
		tmp[1] = ft_strjoin("$", tmp[0]);
		tmp[2] = *line;
		str_replace(line, tmp[1], var_value(tmp[0]));
		free(tmp[2]);
		free(tmp[0]);
		free(tmp[1]);
//		i++;
	}
	tmp[1] = *line;
	tmp[0] = ft_itoa(g_var.last_er);
	str_replace(line, "$?", tmp[0]);
	free(tmp[0]);
	free(tmp[1]);
}
*/
void	handle_tilde(char **line)
{
	int		i;
	int		j;
	char	*tmp;
	char	*tmp2;

	i = 0;
	tmp2 = *line;
	while (line[0][i])
	{
		if (line[0][i] == '~')
		{
			tmp = ft_strjoin(var_value("HOME"),line[0] + i + 1);
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

void	str_builder(char **str, char to_append)
{
	char	*tmp;
	int	len;

//	if(!(**str)) //ft_strncmp(*str, "", 1) == 0
//	{
//		(*str)[0] = to_append;
//		(*str)[1] = 0;
//		return ;
//	}
	len = ft_strlen(*str);
	tmp = ft_calloc(sizeof(*tmp), len + 1);
	ft_memcpy(tmp, *str, len);
	tmp[len] = to_append;
	free(*str);
	*str = ft_calloc(sizeof(*tmp), len + 2);
	ft_memcpy(*str, tmp, len + 1);
	free(tmp);
}

int	is_quote(char c)
{
	return (c == '"' || c == '\'');
}

void	parse_line(char **args, char *line, int len)
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
//		if ((ine[i] == ' ' && !is_between_quote(&line[i], i))
		if ((line[i] == ' ' || line[i] == '\t') && !is_between_quote(&line[i], i))
		{
			if (++i && !(line[i - 2] == ' ' || line[i - 2] == '\t'))
				words++;
			continue;
		}
		str_builder(args + words, line[i++]); // str_builder(&(args)[words], line[i])
//		i++;
	}
	(args)[words + 1] = NULL;
}

t_command	parse(char *line, int fd[2])
{
	t_command	ret;
	int			i;
	char		*tmp;

	// gerer cas "cat << >>" ou "cat << |"
	ret.parse_error = 0;
	handle_tilde(&line);
	handle_var(&line);
	handle_line(line, &ret, fd);
	tmp = line;
	line = ft_strtrim(line, "\t ");
	free(tmp);
	ret.len = ft_countchar(line, ' ') + 1; // TODO \t ?
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
