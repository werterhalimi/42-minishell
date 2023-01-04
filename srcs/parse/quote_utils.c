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

int	is_quote(char c)
{
	return (c == '"' || c == '\'');
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

void	handle_quote(char **split, int len)
{
	int	i;

	if (split[0])
		remove_quote(split[0]);
	i = 1;
	while (i < len)
	{
		if (split[i])
			remove_quote(split[i]);
		i++;
	}
}
