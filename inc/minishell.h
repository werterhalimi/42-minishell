/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 12:58:28 by ncotte            #+#    #+#             */
/*   Updated: 2023/01/04 21:47:49 by shalimi          ###   ########.fr       */
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
/// \param last_er the return (error) code of the last command executed ($?)
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
/// \param argv an array of *char containing the arguments of the command,
/// \n beginning with the command itself and ending with NULL
/// \return 0 on SUCCESS, an error code otherwise
int			env(char *argv[]);

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
int			export_one_var(char *str, int overwrite);

/// \brief pwd bash command (print path of current directory)
/// \param argv an array of *char containing the arguments of the command,
/// \n beginning with the command itself and ending with NULL
/// \return 0 on SUCCESS, 1 if ERROR
int			pwd(char *argv[]);

/// \brief unset bash command (delete environment variables)
/// \param argv an array of *char containing the arguments of the command,
/// \n beginning with the command itself; ending with NULL
/// \return 0 on SUCCESS, 1 if ERROR
int			unset(char *argv[]);

/// \brief Delete one environment variable
/// \param str the name of the variable
/// \param rearrange 0 if no reallocation have to be done, non-0 otherwise
/// \return 0 on SUCCESS, 1 if ERROR
int			unset_one_var(char *name, int rearrange);

/* env */

/// \brief Initialize the global variable structure
/// \param envp an array of *char containing the starting environment variables
/// \return 0 on SUCCESS, 1 if ERROR
int			init(char *envp[]);

/// \brief Search and return the value of an environment variable
/// \param var_name the variable name
/// \return the variable value (as string) if it exist and has a value,
/// \n NULL otherwise
char		*var_value(char const *var_name);

/* exec */

/// \brief Close the file fd except STDIN, STDOUT and STDERR
/// \param fd the file descriptor
void		close_file(int fd);

/// \brief Close all pipes and wait all child process
/// \param pipes the structure containing all pipes
/// \param nb_process the number of child process
/// \param pids an array containing the pid of all child process
void		close_wait(t_pipes pipes, int nb_process, int *pids);

/// \brief Check if a command is a builtin one
/// \param cmd the name of the command
/// \return non-0 if it is, 0 otherwise
int			ft_isbuiltin(char *cmd);

/// \brief Execute a builtin command
/// \param cmd the command structure
/// \return 0 on SUCCESS, non-0 if ERROR
int			exec_builtin(t_command cmd);

/// \brief Main execution
/// \param nb_pipes the total number of pipes
/// \param argv the input of the user splitted by pipes
/// \param files the input and output file descriptors
void		main_exec(int nb_pipes, char **argv, int files[2]);

/// \brief Split the PATH variable
/// \return an array of char * containing all path
char		**env_to_paths(void);

/// \brief Add the complete path of the command to its name
/// \param path an array of char * containing all path
/// \param command the name of the command
/// \return the path of the command, NULL if ERROR
char		*get_path(char **str, char *command);

/// \brief Main signal handler for each situation
void		signals(void);

/* parse */

t_command	parse(char *line, int fd[2]);
//char		*get_string(char **split, char *current, int *i, int len);
int			ft_countchar(const char *str, char c);

/* utils */

/// \brief Copy (duplicate) an array of char*
/// \param src the source
/// \param size the number of elements in src
/// \return the address of the copy, NULL if ERROR
char		**array_copy(char *src[], int size);

/// \brief Print an error message on STDERR of format:
/// \n cmd: arg: msg
/// \param cmd the command causing the error
/// \param arg the argument causing the error
/// \param msg the error message
/// \param errcode the error code to return
/// \return errcode
int			print_error(char const *cmd, char const *arg, \
				char const *msg, int errcode);

/// \brief Print an error message on STDERR of format:
/// \n cmd: `arg': msg
/// \param cmd the command causing the error
/// \param arg the argument causing the error
/// \param msg the error message
/// \param errcode the error code to return
/// \return errcode
int			print_quote_error(char const *cmd, char const *arg, \
				char const *msg, int errcode);

/// \brief Print an error message on STDERR
/// \param error_msg the error message
/// \return 1
int			print_custom_error(char *error_msg);

/// \brief Print an error message on STDERR of format:
///// \n cmd: arg: strerror(errnum)
/// \param cmd the command causing the error
/// \param arg the argument causing the error
/// \param errnum the error number
/// \return errnum converted to an error code
int			print_errno(const char *cmd, const char *arg, int errnum);

/// \brief Free a command structure
/// \param cmd the command
void		free_cmd(t_command cmd);

/// \brief Free an array of char*
/// \param argv the array
void		free_array(char *argv[]);

/// \brief Free the buffer of readline
/// \param buf the buffer containing the input of the user (readline)
/// \return the last error code ($?)
int			free_buffer(char *buf);

/// \brief Free all data
/// \param buf the buffer containing the input of the user (readline)
/// \return the last error code ($?)
int			free_all(char *buf);

char		*ft_trim(char *word, char *set);

/// \brief Remove the first char c in str
/// \param str the source string
/// \param c the char to remove
/// \param index the position of c in str, negative if not specified
/// \return 0 on SUCCESS, 1 if ERROR
int			remove_char(char *str, char c, int index);

/// \brief Sort an array of char* using ASCII value
/// \n from lowest ot biggest (using quicksort)
/// \param argv the array to sort
/// \param size the number of elements to sort
void		sort(char *argv[], long size);

/// Parse
char		**get_commands(char *s, char c, int *len);
int			is_between_quote(char *word, int index);
int			ft_countchar(const char *s, char c);
char		*ft_baskslash(char *str);
char		*join(char *s1, char *s2);
void		set_fd(int *fd, int value);
char		*get_string(char **split, char *current, int index, int len);
void		handle_tilde(char **line);
void		str_replace(char **str, char *to_replace, char *n);
int			is_between_single_quote(char *word, int index);
int			str_replace_once(char **str, char *to_replace, char *n, int index);
void		handle_quote(char **split, int len);
void		remove_quote(char *str);
void		handle_input(char *line, int *fd, t_command *cmd);
void		handle_output(char *line, int *fd, t_command *cmd);
void		remove_quote(char *str);
void		handle_line(char *line, t_command *cmd, int fd[2]);
void		handle_var(char **line);
#endif
