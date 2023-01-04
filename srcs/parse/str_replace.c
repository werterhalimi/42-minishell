/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_replace.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shalimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 18:40:03 by shalimi           #+#    #+#             */
/*   Updated: 2023/01/04 18:54:29 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_and_replace(char **str, char *to_replace, char *new, int len)
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

void	str_replace(char **str, char *to_replace, char *new)
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

int	get_len(char **str, int index, char *to_replace, char *new)
{
	int	len;

	len = ft_strlen(*str);
	if (!ft_strncmp(*str + index, to_replace, ft_strlen(to_replace)))
	{
		len -= ft_strlen(to_replace);
		len += ft_strlen(new);
	}
	return (len);
}

int	str_replace_once(char **str, char *to_replace, char *new, int index)
{
	char	*tmp;
	int		i;
	int		len;
	int		no;

	if (!new)
		new = "";
	len = get_len(str, index, to_replace, new);
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
	*str = tmp;
	return (index + (int) ft_strlen(new));
}
