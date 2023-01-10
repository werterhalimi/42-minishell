/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_tilde.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shalimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 18:34:37 by shalimi           #+#    #+#             */
/*   Updated: 2023/01/04 18:46:38 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*find_and_replace(char **str, char *to_replace, char *new, int len)
{
	char	*tmp;
	int		i;
	int		no;

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
	tmp[len] = 0;
	return (tmp);
}

static void	str_replace(char **str, char *to_replace, char *new)
{
	char	*tmp;
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(*str);
	if (!new)
		new = "";
	while ((*str)[i])
	{
		if (ft_strncmp(*str + i++, to_replace, ft_strlen(to_replace)) == 0)
		{
			len -= ft_strlen(to_replace);
			len += ft_strlen(new);
		}
	}
	tmp = find_and_replace(str, to_replace, new, len);
	*str = tmp;
}

static void	remove_space(char *tmp)
{
	int	j;

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
}

void	handle_tilde(char **line)
{
	int		i;
	char	*tmp;
	char	*tmp2;

	i = 0;
	tmp2 = *line;
	while (line[0][i])
	{
		if (line[0][i] == '~')
		{
			tmp = ft_strjoin(var_value("HOME"), line[0] + i + 1);
			remove_space(tmp);
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
