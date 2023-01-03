/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 13:42:06 by ncotte            #+#    #+#             */
/*   Updated: 2022/12/20 13:42:07 by ncotte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// \brief Calculate the difference s1 - s2 lexicographically (as unsigned char)
/// \param s1 the first string
/// \param s2 the second string
/// \return the difference
static int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] == s2[i] && (s1[i] || s2[i]))
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

/// \brief Swap 2 string
/// \param start the address of the first string
/// \param end the address of the second string
static void	swap(char **start, char **end)
{
	char	*tmp;

	tmp = *start;
	*start = *end;
	*end = tmp;
}

/// \brief Quicksort algorithm for an array of char*
/// \param pivot the address of the pivot
/// \param start the address of the first string
/// \param end the address of the last string
/// \return the new address of the pivot
static char	**quicksort(char **pivot, char **start, char **end)
{
	if (start < end)
	{
		if (ft_strcmp(*start, *pivot) >= 0 && ft_strcmp(*end, *pivot) >= 0)
			return (quicksort(pivot, start, --end));
		else if (ft_strcmp(*start, *pivot) < 0 && ft_strcmp(*end, *pivot) < 0)
			return (quicksort(pivot, ++start, end));
		else if (ft_strcmp(*start, *pivot) >= 0 && ft_strcmp(*end, *pivot) <= 0)
			swap(start, end);
		return (quicksort(pivot, ++start, --end));
	}
	else if (start == end && ft_strcmp(*start, *pivot) > 0)
		swap(pivot, --end);
	else
		swap(pivot, end);
	return (end);
}

void	sort(char *argv[], long size)
{
	char	**pivot;
	long	new_size;

	pivot = argv;
	if (size > 1)
	{
		pivot = quicksort(pivot, argv + 1, argv + size - 1);
		new_size = pivot - argv;
		sort(argv, new_size);
		sort(pivot + 1, size - new_size - 1);
	}
}
