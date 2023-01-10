/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shalimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 18:56:57 by shalimi           #+#    #+#             */
/*   Updated: 2023/01/04 20:46:52 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_between_quote(char const *word, int index)
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

int	is_between_single_quote(char const *word, int index)
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
