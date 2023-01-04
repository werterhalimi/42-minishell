/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shalimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 17:21:26 by shalimi           #+#    #+#             */
/*   Updated: 2023/01/04 18:57:18 by shalimi          ###   ########.fr       */
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

void	set_fd(int *fd, int value)
{
	if (*fd != STDIN_FILENO && *fd != STDOUT_FILENO && *fd != STDERR_FILENO)
		return ;
	*fd = value;
}
