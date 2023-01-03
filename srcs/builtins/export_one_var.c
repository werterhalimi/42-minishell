/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_one_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 18:29:48 by ncotte            #+#    #+#             */
/*   Updated: 2023/01/02 18:29:52 by ncotte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// \brief Add an environment variable
/// \param str the name and value of the variable (name=value)
/// \param nb_variables the total number of variables
/// \n (not including the new one)
/// \return 0 on SUCCESS, 1 if ERROR
static int	add_variable(char *str, int nb_variables)
{
	char	**tmp;
	int		i;

	tmp = malloc(sizeof (*tmp) * (nb_variables + 2));
	if (!tmp)
		return (ERROR);
	i = -1;
	while (g_var.envp[++i])
		tmp[i] = g_var.envp[i];
	tmp[i] = ft_strdup(str);
	if (!tmp[i])
	{
		free(tmp);
		return (ERROR);
	}
	tmp[++i] = NULL;
	free(g_var.envp);
	g_var.envp = tmp;
	return (SUCCESS);
}

/// \brief Update an environment variable
/// \param str the name and value of the variable (name=value)
/// \param index the position of the variable ib the array
/// \param append 1 if append mode, overwrite/replace mode otherwise
/// \param value_pos the position of the start of the variable's value
/// \n (right after '=')
/// \return 0 on SUCCESS, 1 if ERROR
static int	update_variable(char *str, int index, int append, int value_pos)
{
	char	*tmp;

	if (append == YES)
		tmp = ft_strjoin(g_var.envp[index], str + value_pos);
	else
		tmp = ft_strdup(str);
	if (!tmp)
		return (ERROR);
	free(g_var.envp[index]);
	g_var.envp[index] = tmp;
	return (SUCCESS);
}

int	export_one_var(char *str, int overwrite)
{
	int	i;
	int	j;
	int	append;

	i = 0;
	append = NO;
	while (str[i] && str[i] != '=')
		if (!(ft_isalnum(str[i]) || (str[i] == '+' && str[i + 1] == '=')) \
			|| i++ < 0)
			return (print_quote_error("minishell: export", str, \
				"not a valid identifier", ERROR));
	if (overwrite == NO && str[i] == '=' && str[i - 1] == '+' \
		&& !remove_char(str, '+', --i))
		append = YES;
	j = 0;
	while (g_var.envp[j] && (ft_strncmp(g_var.envp[j], str, i) \
		|| (g_var.envp[j][i] && g_var.envp[j][i] != '=')))
		j++;
	if (!g_var.envp[j])
		return (add_variable(str, j));
	else if (overwrite == YES || str[i++] == '=')
		return (update_variable(str, j, append, i));
	return (SUCCESS);
}
