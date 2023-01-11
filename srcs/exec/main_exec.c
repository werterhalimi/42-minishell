/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shalimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 15:52:46 by shalimi           #+#    #+#             */
/*   Updated: 2022/12/30 18:59:17 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// \brief Copy the content of a pair of int from src to dest
/// \param dest destination
/// \param src source
static void	copy(int dest[2], int const src[2])
{
	dest[0] = src[0];
	dest[1] = src[1];
}

/// \brief Execution of the child process
/// \param cmd the command structure
/// \param out the output pipe
/// \return 0 on SUCCESS, non-0 if ERROR
static int	child_process(t_command cmd, int *out)
{
	char	**paths;

	signals();
	if (cmd.fd[0] < 0)
	{
		print_errno("minishell", cmd.command, cmd.errnum);
		return (ERROR);
	}
	close_file(out[0]);
	paths = env_to_paths();
	dup2(cmd.fd[0], 0);
	dup2(cmd.fd[1], 1);
	if (ft_isbuiltin(cmd.command))
		return (exec_builtin(cmd));
	else if (!get_path(paths, cmd.command))
		return (PATH_ERROR);
	else if (execve(get_path(paths, cmd.command), cmd.args, g_var.envp))
		return (print_errno(cmd.command, cmd.args[1], errno));
	return (SUCCESS);
}

/// \brief Execution of a sub-process
/// \param in the input pipe
/// \param out the output pipe
/// \param args a string containing the command and its arguments
/// \param nb_pipes the number of child process
/// \return the pid of the child process
static int	sub_process(int in[2], int out[2], char *args, int nb_pipes)
{
	t_command	cmd;

	args = ft_strtrim(args, " 	");
	cmd = parse(args, (int [2]){in[0], out[1]});
	if (cmd.parse_error || g_var.quit_child == YES || !cmd.command[0])
	{
		free_cmd(cmd);
		if (cmd.parse_error)
			return (cmd.parse_error);
		return (EXCEPTION);
	}
	if (!nb_pipes && ft_isbuiltin(cmd.command))
	{
		g_var.last_er = exec_builtin(cmd);
		free_cmd(cmd);
		return (EXCEPTION);
	}
	g_var.pid = fork();
	if (!g_var.pid)
		exit(child_process(cmd, out));
	if (nb_pipes)
		g_var.exit = NO;
	close_file(out[1]);
	free_cmd(cmd);
	return (g_var.pid);
}

/// \brief Execution loop for each process
/// \param index an id that counts the number of process
/// \param argv the input of the user splitted by pipes
/// \param pids an array of int containing for all child pids
/// \param pipes the pipes structure
static void	exec_loop(int *index, char **argv, int *pids, t_pipes *pipes)
{
	while (*index <= pipes->nb_pipes)
	{
		if (*index == 0)
		{
			pids[*index] = sub_process(pipes->files, pipes->in, \
				argv[*index + 0], pipes->nb_pipes);
			close_file((pipes->files)[0]);
			if (pipes->nb_pipes == 1)
				copy(pipes->out, pipes->in);
		}
		else if (*index == pipes->nb_pipes)
			pids[*index] = sub_process(pipes->out, pipes->files, \
				argv[*index], pipes->nb_pipes);
		else
		{
			if (*index > 1)
				copy(pipes->in, pipes->out);
			pipe(pipes->out);
			pids[*index] = sub_process(pipes->in, pipes->out, \
				argv[*index], pipes->nb_pipes);
		}
		if (pids[(*index)++] < -1)
			break ;
	}
}

void	main_exec(int nb_pipes, char **argv, int files[2])
{
	int		*pids;
	int		index;
	t_pipes	pipes;

	if (!argv)
		return ;
	g_var.status = EXECUTE;
	pids = malloc(sizeof(int) * (nb_pipes + 1));
	if (pids)
	{
		index = 0;
		pipes.nb_pipes = nb_pipes;
		copy(pipes.files, files);
		pipe(pipes.in);
		if (!nb_pipes)
			copy(pipes.in, files);
		exec_loop(&index, argv, pids, &pipes);
		close_wait(pipes, index, pids);
		g_var.quit_child = NO;
		g_var.pid = 0;
	}
	free_array(argv);
	g_var.status = READ;
}
