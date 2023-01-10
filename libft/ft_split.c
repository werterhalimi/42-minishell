/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 15:28:17 by ncotte            #+#    #+#             */
/*   Updated: 2022/10/12 14:52:34 by ncotte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/// @brief Free allocated memory in result of size elements and tab
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
/// @return its address, NULL if error
static int	*ft_parsing(char const *str, char c, int *size)
{
	int	*tab;
	int	i;

	tab = malloc(sizeof (*tab) * ft_strlen(str) * 2);
	if (tab)
	{
		i = 0;
		while (str[i])
		{
			while (str[i] && str[i] == c)
				i++;
			if (str[i])
				tab[(*size) * 2] = i;
			while (str[i] && str[i] != c)
				i++;
			if (str[i - 1] != c)
			{
				tab[(*size) * 2 + 1] = i - tab[(*size) * 2];
				++(*size);
			}
		}
	}
	return (tab);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	int		size;
	int		i;
	int		*tab;

	size = 0;
	tab = ft_parsing(s, c, &size);
	if (!tab)
		return (NULL);
	result = malloc(sizeof (*result) * (size + 1));
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
	}
	free(tab);
	return (result);
}
