/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 16:44:10 by ncotte            #+#    #+#             */
/*   Updated: 2022/12/12 22:20:17 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	free_all(char *buf)
{
	free(buf);
	ft_lstclear(&(g_var.envp), free);
	return (0);
}

int	free_buffer(t_list **buffer)
{
	ft_lstclear(buffer, free);
	return (0);
}
