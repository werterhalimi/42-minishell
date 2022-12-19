/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 12:58:28 by ncotte            #+#    #+#             */
/*   Updated: 2022/12/19 23:24:40 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <unistd.h>
# include <fcntl.h>
# include <signal.h>
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
	t_list	**parse_alloc;
	int		*last_er;
}	t_global;

typedef struct s_command
{
	char	*command;
	char	**args;
	int		argc;
	int		fd[2];
}	t_command;

t_global	g_var;

/* Alloc */

void	*ft_alloc(int size, int len, t_list **buff);
int		free_buffer(t_list **buffer);

/* builtins */

int			ft_isbuiltin(char *cmd);

int			env(void);

int			pwd(void);

int			export(char *str);

int			unset(char *str);

/* utils */

int			init(char *envp[]);

char		*prompt(void);

int			free_all(char *buf);

int			print_error(char *error_msg);

/* parse */

t_command	parse(char *line, int fd[2]);
char		*get_string(char **split, char *current, int *i, int len);
int			ft_countchar(const char *str, char c);

/* Exec */

int		ft_find_paths_index(char **str);
char	*get_path(char **str, char *command);
char	**env_to_paths(char **env);
void	launch_pipex(int argc, char **argv, char **env, int files[2]);
int		execute(t_command instr, char *envp[]);

#endif
