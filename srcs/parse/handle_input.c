/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shalimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 19:24:49 by shalimi           #+#    #+#             */
/*   Updated: 2023/01/11 20:20:07 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_input_file(char *line, int *fd, char *tmp, t_command *cmd)
{
	char	**split;
	int		f[2];
	char	*tpmp;

	line = &line[1];
	while (*line == ' ')
		line++;
	split = ft_split(line, ' ');
	line = ft_strtrim(line, "\t ");
	tpmp = get_string(split, line, 0, ft_strlen(line));
	cmd->errnum = set_fd(&fd[0], open(tpmp, O_RDONLY));
	free(tpmp);
	f[0] = 0;
	f[1] = 0;
	while (f[0] < (int) ft_strlen(tmp))
	{
		if (tmp[f[0]] != ' ')
			f[1] = -1;
		else if (f[1] == -1)
			break ;
		tmp[f[0]] = ' ';
		f[0]++;
	}
	free_array(split);
}

static void	handle_heredoc(char *sep, char *buff, int *fd)
{
	int	f[2];

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
			if (!buff || (!ft_strncmp(buff, sep, ft_strlen(buff))
					&& ft_strlen(buff) == ft_strlen(sep)))
				exit(SUCCESS);
			ft_putendl_fd(buff, f[1]);
		}
	}
	signals();
	g_var.status = EXECUTE;
	close(f[1]);
	set_fd(&fd[0], f[0]);
}

static int	is_valid_sep(char *sep)
{
	return (ft_strncmp(sep, ">", ft_strlen(sep)) != 0 && \
			ft_strncmp(sep, "<", ft_strlen(sep)) != 0 && \
			ft_strncmp(sep, ">>", ft_strlen(sep)) != 0 && \
			ft_strncmp(sep, "<<", ft_strlen(sep)) != 0);
}

static int	handle_input_heredoc(char *line, int *fd, char *tmp)
{
	char	**split;
	char	*sep;
	int		f[2];

	line = &line[2];
	while (*line == ' ')
		line++;
	split = ft_split(line, ' ');
	line = ft_strtrim(line, "\t ");
	sep = get_string(split, line, 0, ft_strlen(line));
	if (!sep || !is_valid_sep(sep))
		return (-258);
	else
	{
		f[0] = 0;
		while (f[0] < (int) ft_strlen(tmp))
		{
			tmp[f[0]] = ' ';
			f[0]++;
		}
		handle_heredoc(sep, "", fd);
	}
	free(split);
	free(line);
	return (0);
}

void	handle_input(char *line, int *fd, t_command *cmd)
{
	char	*tmp;

	tmp = line;
	line = ft_strtrim(line, "\t ");
	if (ft_strlen(line) < 2 || (ft_strlen(line) == 2 && line[0] == line[1]))
	{
		free(line);
		cmd->parse_error = SYNTAX_ERROR * (-1);
		return ;
	}
	if (line[1] == '<')
		cmd->parse_error = handle_input_heredoc(line, fd, tmp);
	else
		handle_input_file(line, fd, tmp, cmd);
	free(line);
}
