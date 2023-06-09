/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_var_name.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 16:48:03 by ncotte            #+#    #+#             */
/*   Updated: 2023/01/10 16:48:05 by ncotte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	valid_var_name(char c, int index)
{
	return ((!index && (ft_isalpha(c) || c == '_')) \
		|| (index && (ft_isalnum(c) || c == '_')));
}
