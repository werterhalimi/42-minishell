/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_trim.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shalimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 22:29:01 by shalimi           #+#    #+#             */
/*   Updated: 2022/12/29 22:31:56 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_trim(char *word, char *set)
{
	char	*tmp;
	char	*ret;

	tmp = word;
	ret = ft_strtrim(word, set);
	free(tmp);
	return (ret);
}
