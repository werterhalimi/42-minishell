/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isbuiltin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shalimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 14:27:36 by shalimi           #+#    #+#             */
/*   Updated: 2022/12/15 14:35:22 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isbuiltin(char *cmd)
{
	return (!ft_strncmp(cmd, "echo", 5) \
		|| !ft_strncmp(cmd, "cd", 3) \
		|| !ft_strncmp(cmd, "pwd", 4) \
		|| !ft_strncmp(cmd, "export", 7) \
		|| !ft_strncmp(cmd, "unset", 6) \
		|| !ft_strncmp(cmd, "env", 4) \
		|| !ft_strncmp(cmd, "exit", 5));
}
