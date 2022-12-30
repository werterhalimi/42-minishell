/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 12:58:28 by ncotte            #+#    #+#             */
/*   Updated: 2022/12/27 22:28:52 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>
# include <fcntl.h>
# include <signal.h>
# include <limits.h>
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

# define SUCCESS			0
# define ERROR				1
# define PERM_ERROR			126
# define PATH_ERROR			127
# define QUIT_CHILD			131
# define UNDEFINED_ERROR	255
# define SYNTAX_ERROR		258

# define READ		0
# define EXECUTE	1
# define HEREDOC	2

# define NO		0
# define YES	1

# define NO_WAIT	-1

typedef struct s_global
{
	char	**envp;
	t_list	**parse_alloc;
	int		last_er;
	int		exit;
	int		pid;
	int		status;
	int		quit_child;
}	t_global;

typedef struct s_command
{
	char	*command;
	char	**args;
	int		parse_error;
	int		fd[2];
}	t_command;

extern t_global	g_var;

/* readline */

//void		rl_clear_history(void);

//void		rl_replace_line(const char *, int);

void		print_cmd(t_command cmd);

/* Alloc */

void		*ft_alloc(int size, size_t len, t_list **buff);

/* builtins */

int			env(char const *arg);

int			pwd(void);

int			export(char *str);

int			unset(char *str);

int			echo(char *argv[]);

int			cd(char *argv[]);

int			ft_exit(char *argv[]);

/* utils */

void		free_envp(void);

int			free_all(char *buf);

int			free_buffer(char *buf);

int			print_error(char const *cmd, char const *arg, \
				char const *msg, int errcode);

int			print_quote_error(char const *cmd, char const *arg, \
				char const *msg, int errcode);

int			print_custom_error(char *error_msg);

int			print_errno(const char *cmd, const char *arg, int errnum);

void		sort(char *argv[], long size);

char		**array_copy(char *src[], int size);

int			remove_char(char *str, char c, int index);

char		*ft_trim(char *word, char *set);

/* env */

int			init(char *envp[]);

char		*var_value(char const *var_name);

/* parse */

t_command	parse(char *line, int fd[2]);
char		*get_string(char **split, char *current, int *i, int len);
int			ft_countchar(const char *str, char c);

/* exec */

char		*get_path(char **str, char *command);

char		**env_to_paths(void);

void		signals(void);

void		close_file(int i);

void		close_wait(int fd[2], int out[2], int j, int *pid);

int			ft_isbuiltin(char *cmd);

int			exec_builtin(t_command instr);

void		launch_pipex(int argc, char **argv, int files[2]);

#endif
