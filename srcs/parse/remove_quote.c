/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shalimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 20:48:27 by shalimi           #+#    #+#             */
/*   Updated: 2023/01/04 21:31:47 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count(int no, int i, char *str)
{
	no--;
	ft_memmove(str + i, str + 1 + i, ft_strlen(str + 1 + i));
	str[ft_strlen(str) - 1] = 0;
	return (no);
}

void	find_and_remove_quote(char *str, int no_double, int no_single, int i)
{
	while (str[i])
	{
		if (str[i] == '"')
		{
			if (no_double == 0)
			{
				i++;
				continue ;
			}
			no_single = count(no_double, i, str);
			continue ;
		}
		if (str[i] == '\'')
		{
			if (no_single == 0)
			{
				i++;
				continue ;
			}
			no_single = count(no_single, i, str);
			continue ;
		}
		i++;
	}
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
	find_and_remove_quote(str, no_double, no_single, 0);
	str[final_len] = 0;
}
