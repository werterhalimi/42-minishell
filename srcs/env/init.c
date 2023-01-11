/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 16:43:51 by ncotte            #+#    #+#             */
/*   Updated: 2022/12/08 17:35:45 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// \brief Generate a custom PROMPT variable
/// \return a string containing the prompt on SUCCESS, NULL if ERROR
static char	*prompt(void)
{
	char	*prompt1;
	char	*prompt2;

	prompt1 = ft_strjoin("PROMPT=", CYAN);
	if (!prompt1)
		return (NULL);
	prompt2 = ft_strjoin(prompt1, "minishell > ");
	free(prompt1);
	if (!prompt2)
		return (NULL);
	prompt1 = ft_strjoin(prompt2, RESET_COLOR);
	free(prompt2);
	return (prompt1);
}

/// \brief Copy (duplicate) an array of char*
/// \param src the source
/// \param size the number of elements in src
/// \return the address of the copy, NULL if ERROR
static char	**array_copy(char *src[], int size)
{
	char	**copy;
	int		i;

	copy = malloc(sizeof (*copy) * (size + 1));
	if (copy)
	{
		i = 0;
		while (i < size)
		{
			copy[i] = ft_strdup(src[i]);
			if (!copy[i++])
			{
				while (--i >= 0)
					free(copy[i]);
				free(copy);
				return (NULL);
			}
		}
		copy[i] = NULL;
	}
	return (copy);
}

/// \brief Copy the environment variables
/// \n and initialize the global variable structure
/// \param envp an array of *char containing the starting environment variables
/// \return 0 on SUCCESS, 1 if ERROR
static int	init_global_variable(char *envp[])
{
	int	i;

	g_var.exit = NO;
	g_var.status = READ;
	g_var.pid = 0;
	g_var.quit_child = NO;
	g_var.last_er = ERROR;
	i = 0;
	while (envp[i])
		i++;
	g_var.envp = array_copy(envp, i);
	if (!g_var.envp)
		return (ERROR);
	return (SUCCESS);
}

/// \brief Create a copy of the current terminal options
/// \n and prevent control characters to be printed
/// \param termios_new the structure containing the new options
/// \param termios_copy the structure containing the original options
/// \return 0 on SUCCESS, 1 if ERROR
static int	init_termios(t_termios *termios_new, t_termios *termios_copy)
{
	if (tcgetattr(STDIN_FILENO, termios_new))
		return (ERROR);
	*termios_copy = *termios_new;
	termios_new->c_lflag &= ~ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, termios_new))
		return (ERROR);
	return (SUCCESS);
}

int	init(char *envp[], t_termios *termios_new, t_termios *termios_copy)
{
	int		res;
	char	*tmp;

	if (init_global_variable(envp))
		return (free_all(NULL, NULL));
	if (export_one_var("OLDPWD", YES))
		return (free_all(NULL, NULL));
	tmp = prompt();
	if (!tmp)
		return (free_all(NULL, NULL));
	res = export_one_var(tmp, YES);
	free(tmp);
	if (res)
		return (free_all(NULL, NULL));
	if (isatty(STDIN_FILENO) && init_termios(termios_new, termios_copy))
		return (free_all(NULL, NULL));
	g_var.last_er = SUCCESS;
	return (SUCCESS);
}
