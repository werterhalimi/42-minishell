/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 16:43:57 by ncotte            #+#    #+#             */
/*   Updated: 2022/12/08 16:43:58 by ncotte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	print_error(char *error_msg)
{
	ft_putstr_fd(RED, STDERR_FILENO);
	ft_putendl_fd(error_msg, STDERR_FILENO);
	ft_putstr_fd(RESET_COLOR, STDERR_FILENO);
	free_all(NULL);
	return (1);
}
