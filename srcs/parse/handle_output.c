/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_output.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shalimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 20:22:14 by shalimi           #+#    #+#             */
/*   Updated: 2023/01/04 20:40:35 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	append_and_clear(char *line, int *fd, int append, char *tmp)
{
	char	**split;
	int		i;

	split = ft_split(line, ' ');
	line = ft_strtrim(line, "\t ");
	if (!append)
		set_fd(&fd[1], open(get_string(split, line, 0,
					ft_strlen(line)), O_WRONLY | O_CREAT | O_TRUNC, 0644));
	else
		set_fd(&fd[1], open(get_string(split, line, 0,
					ft_strlen(line)), O_WRONLY | O_CREAT | O_APPEND, 0644));
	append = 0;
	i = 0;
	while (i < (int) ft_strlen(tmp))
	{
		if (ft_isalnum(tmp[i]))
			append = -1;
		else if (append == -1)
			break ;
		tmp[i] = ' ';
		i++;
	}
	free_array(split);
	free(line);
}

void	handle_output(char *line, int *fd, t_command *cmd)
{
	int		append;
	char	*tmp;

	tmp = line;
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
	while (*line == ' ')
		line++;
	append_and_clear(line, fd, append, tmp);
}
