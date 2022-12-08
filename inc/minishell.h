/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 12:58:28 by ncotte            #+#    #+#             */
/*   Updated: 2022/12/07 12:58:30 by ncotte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <unistd.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>

# define RESET_COLOR	"\033[0m"
# define BLACK			"\033[0m\033[30m"
# define RED			"\033[0m\033[31m"
# define GREEN			"\033[0m\033[32m"
# define YELLOW			"\033[0m\033[33m"
# define BLUE			"\033[0m\033[34m"
# define MAGENTA		"\033[0m\033[35m"
# define CYAN			"\033[0m\033[36m"
# define WHITE			"\033[0m\033[37m"
# define BOLD_BLACK		"\033[1m\033[30m"
# define BOLD_RED		"\033[1m\033[31m"
# define BOLD_GREEN		"\033[1m\033[32m"
# define BOLD_YELLOW	"\033[1m\033[33m"
# define BOLD_BLUE		"\033[1m\033[34m"
# define BOLD_MAGENTA	"\033[1m\033[35m"
# define BOLD_CYAN		"\033[1m\033[36m"
# define BOLD_WHITE		"\033[1m\033[37m"

typedef struct s_global
{
	t_list	*envp;
}	t_global;

t_global	g_var;

/* builtins */

int		env(void);

int		pwd(void);

int		export(char *str);

int		unset(char *str);

/* utils */

int		init(char *envp[]);

char	*prompt(void);

int		free_all(char *buf);

int		print_error(char *error_msg);

#endif
