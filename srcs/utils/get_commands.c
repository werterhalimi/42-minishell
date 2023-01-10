/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_commands.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shalimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 20:45:04 by shalimi           #+#    #+#             */
/*   Updated: 2023/01/03 21:22:18 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Free allocated memory in result of size elements and tab
/// \param result an array of char to free
/// \param size the number of element of result to free
/// \param tab an array of int to free
/// \return the NULL pointer
static void	*ft_free(char **result, int size, int *tab)
{
	int	i;

	i = 0;
	while (i < size)
		free(result[i++]);
	free(result);
	free(tab);
	return (NULL);
}

/// @brief Create an array containing the lengths of sub-string in str between c
/// \n (not including those between single or double quotes)
/// \param str the string to parse
/// \param c the delimiting char
/// \param size the address of the number of elements of the parse
/// @return its address, NULL if error
static int	*ft_parsing(char *str, char c, int *size)
{
	int	*tab;
	int	i;

	tab = malloc(sizeof (*tab) * ft_strlen(str) * 2);
	if (tab)
	{
		i = 0;
		while (str[i])
		{
			while (str[i] && str[i] == c && !is_between_quote(str + i, i))
				i++;
			if (str[i])
				tab[(*size) * 2] = i;
			while (str[i] && (str[i] != c || is_between_quote(str + i, i)))
				i++;
			if (str[i - 1] != c || is_between_quote(str + i, i - 1))
			{
				tab[(*size) * 2 + 1] = i - tab[(*size) * 2];
				++(*size);
			}
		}
	}
	return (tab);
}

char	**get_commands(char *s, char c, int *len)
{
	char	**result;
	int		size;
	int		*tab;
	int		i;

	size = 0;
	tab = ft_parsing(s, c, &size);
	if (!tab)
		return (NULL);
	result = malloc(sizeof(*result) * (size + 1));
	if (result)
	{
		i = 0;
		while (i < size)
		{
			result[i] = ft_substr(s, tab[i * 2], tab[i * 2 + 1]);
			if (!result[i++])
				return (ft_free(result, i - 1, tab));
		}
		result[i] = NULL;
		*len = i - 1;
	}
	free(tab);
	return (result);
}
