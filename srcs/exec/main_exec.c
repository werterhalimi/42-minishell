/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shalimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 15:52:46 by shalimi           #+#    #+#             */
/*   Updated: 2022/12/29 23:23:09 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	copy(int dest[2], int const src[2])
{
	dest[0] = src[0];
	dest[1] = src[1];
}

static int	child_process(t_command cmd, int *out)
{
	char	**paths;

	signals();
	if (cmd.fd[0] < 0)
		return (print_error("minishell", cmd.command, \
				"No such file or directory", ERROR));
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

int	middle_process(int in[2], int out[2], char *args, int argc)
{
	t_command	cmd;

	args = ft_strtrim(args, " 	");
	cmd = parse(args, (int [2]){in[0], out[1]});
	free(args);
	if (cmd.parse_error || g_var.quit_child == YES || !cmd.command[0])
	{
		free(cmd.args);
		if (cmd.parse_error)
			return (cmd.parse_error);
		return (NO_WAIT);
	}
	if (!argc && ft_isbuiltin(cmd.command))
	{
		g_var.last_er = exec_builtin(cmd);
		free(cmd.args);
		return (NO_WAIT);
	}
	g_var.pid = fork();
	if (!g_var.pid)
		exit(child_process(cmd, out));
	if (argc)
		g_var.exit = NO;
	close_file(out[1]);
	free(cmd.args);
	return (g_var.pid);
}
/*
int	launch_process(int in[2], int out[2], char *arg, int argc)
{
	pipe(out);
	return (middle_process(in, out, arg, argc));
}

static void	loop()
{

}
*/
void	launch_pipex(int argc, char **argv, int files[2])
{
	int	*pids;
	int	in[2];
	int	out[2];
	int	j;

	g_var.status = EXECUTE;
	pids = malloc(sizeof(int) * (argc + 1));
	if (pids)
	{
		j = 0;
		pipe(in);
		if (!argc)
			copy(in, files);
		while (j <= argc)
		{
			if (j == 0)
			{
				pids[j] = middle_process(files, in, argv[j + 0], argc);
				close_file(files[0]);
				if (argc == 1)
					copy(out, in);
			}
			else if (j == argc)
				pids[j] = middle_process(out, files, argv[j], argc);
			else
			{
				if (j > 1) // ???
					copy(in, out);
				pipe(out);
				pids[j] = middle_process(in, out, argv[j], argc);
			}
			if (pids[j++] < -1)
				break ;
		}
		close_wait(in, out, j, &pids);
		g_var.quit_child = NO;
		g_var.pid = 0;
	}
	free(argv);
	g_var.status = READ;
}
