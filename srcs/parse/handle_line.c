/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shalimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 21:32:53 by shalimi           #+#    #+#             */
/*   Updated: 2023/01/04 21:46:27 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	map(char *line, int len, t_command *cmd, int *fd)
{
	char	c;

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
}

void	handle_line(char *line, t_command *cmd, int fd[2])
{
	int		len;

	len = ft_strlen(line);
	while (len >= 0)
	{
		if (is_between_quote(line + len, len))
		{
			len--;
			continue ;
		}
		map(line, len, cmd, fd);
		len--;
	}
	cmd->fd[0] = fd[0];
	cmd->fd[1] = fd[1];
	if (cmd->parse_error == SYNTAX_ERROR * -1)
		ft_putendl_fd("syntax error near unexpected token 'newline'", \
			STDERR_FILENO);
}
