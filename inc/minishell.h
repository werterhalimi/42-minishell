/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 12:58:28 by ncotte            #+#    #+#             */
/*   Updated: 2022/12/30 19:15:21 by shalimi          ###   ########.fr       */
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

/* Color codes */

# define RESET_COLOR		"\033[0m"
# define BLACK				"\033[0m\033[30m"
# define RED				"\033[0m\033[31m"
# define GREEN				"\033[0m\033[32m"
# define YELLOW				"\033[0m\033[33m"
# define BLUE				"\033[0m\033[34m"
# define MAGENTA			"\033[0m\033[35m"
# define CYAN				"\033[0m\033[36m"
# define WHITE				"\033[0m\033[37m"
# define BOLD_BLACK			"\033[1m\033[30m"
# define BOLD_RED			"\033[1m\033[31m"
# define BOLD_GREEN			"\033[1m\033[32m"
# define BOLD_YELLOW		"\033[1m\033[33m"
# define BOLD_BLUE			"\033[1m\033[34m"
# define BOLD_MAGENTA		"\033[1m\033[35m"
# define BOLD_CYAN			"\033[1m\033[36m"
# define BOLD_WHITE			"\033[1m\033[37m"

/* Error/return codes */

# define SUCCESS			0
# define ERROR				1
# define PERM_ERROR			126
# define PATH_ERROR			127
# define QUIT_CHILD			131
# define UNDEFINED_ERROR	255
# define SYNTAX_ERROR		258

/* Status code */

# define READ				0
# define EXECUTE			1
# define HEREDOC			2

/* Bool codes */

# define NO					0
# define YES				1

/* Special/exception code */

# define EXCEPTION			-1

/* Types/structures */

/// \brief Global variable structure
/// \param envp an array of *char containing the environment variables,
/// \n ending with NULL
/// \param last_er the return (error) code of the last command executed
/// \param exit true if an exit command was called, false otherwise
/// \param pid the pid of the child process for the parent, 0 otherwise
/// \param status the global status (READ, EXECUTE, HEREDOC)
/// \param quit_child true if a process was killed
/// \n or if no process should be launched, false otherwise
typedef struct s_global
{
	char	**envp;
	int		last_er;
	int		exit;
	int		pid;
	int		status;
	int		quit_child;
}	t_global;

/// \brief Command structure
/// \param command the name of the command
/// \param args an array of *char containing the arguments of the command,
/// \n beginning with the command itself and ending with NULL
/// \param len the size of args
/// \param parse_error 0 if no error, negative otherwise
/// \param fd the input and output file descriptors for the command
typedef struct s_command
{
	char	*command;
	char	**args;
	int		len;
	int		parse_error;
	int		fd[2];
}	t_command;

/// \brief Pipes structure
/// \param nb_pipes the total number of pipes
/// \param files the global input and output file descriptors
/// \param in the input pipe
/// \param out the output pipe
typedef struct s_pipes
{
	int	nb_pipes;
	int	files[2];
	int	in[2];
	int	out[2];
}	t_pipes;

/// \brief The global variable
/// \param envp an array of *char containing the environment variables,
/// \n ending with NULL
/// \param last_er the return (error) code of the last command executed
/// \param exit true if an exit command was called, false otherwise
/// \param pid the pid of the child process for the parent, 0 otherwise
/// \param status the global status (READ, EXECUTE, HEREDOC)
/// \param quit_child true if a process was killed
/// \n or if no process should be launched, false otherwise
extern t_global	g_var;

/* readline */

//void		rl_clear_history(void);

//void		rl_replace_line(const char *, int);

void		print_cmd(t_command cmd);

/* builtins */

/// \brief cd bash command (change directory)
/// \param argv an array of *char containing the arguments of the command,
/// \n beginning with the command itself and ending with NULL
/// \return 0 on SUCCESS, 1 if ERROR
int			cd(char *argv[]);

/// \brief echo bash command (print its arguments)
/// \param argv an array of *char containing the arguments of the command,
/// \n beginning with the command itself and ending with NULL
/// \return 0 on SUCCESS, 1 if ERROR (no error ?)
int			echo(char *argv[]);

/// \brief env bash command (print the initialized environment variables)
/// \param arg the first argument after the command's name
/// \return 0 on SUCCESS, an error code otherwise
int			env(char const *arg);

/// \brief exit bash command (terminate the process)
/// \param argv an array of *char containing the arguments of the command,
/// \n beginning with the command itself and ending with NULL
/// \return the first given error code modulo 256
int			ft_exit(char *argv[]);

/// \brief export bash command (create/update environment variables)
/// \param argv an array of *char containing the arguments of the command,
/// \n beginning with the command itself; ending with NULL
/// \return 0 on SUCCESS, 1 if ERROR
int			export(char *argv[]);

/// \brief Create/update one environment variable
/// \param str the name and value of the variable (name=value)
/// \return 0 on SUCCESS, 1 if ERROR
int			export_one_var(char *str);

/// \brief pwd bash command (print path of current directory)
/// \return 0 on SUCCESS, 1 if ERROR
int			pwd(void);

/// \brief unset bash command (delete environment variables)
/// \param argv an array of *char containing the arguments of the command,
/// \n beginning with the command itself; ending with NULL
/// \return 0 on SUCCESS, 1 if ERROR
int			unset(char *argv[]);

/// \brief Delete one environment variable
/// \param str the name of the variable
/// \param rearrange 0 if no reallocation have to be done, non-0 otherwise
/// \return 0 on SUCCESS, 1 if ERROR
int			unset_one_arg(char *name, int rearrange);

/* utils */

void		free_cmd(t_command cmd);

void		free_array(char *argv[]);

int			free_buffer(char *buf);

int			free_all(char *buf);

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

void		close_wait(t_pipes pipes, int nb_process, int *pids);

int			ft_isbuiltin(char *cmd);

int			exec_builtin(t_command instr);

void		launch_pipex(int nb_pipes, char **argv, int files[2]);

#endif
