/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shalimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 15:52:46 by shalimi           #+#    #+#             */
/*   Updated: 2022/12/19 00:13:33 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	middle_process(int in[3], int out[2], char *args, char **env);

int	launch_process(int in[2], int out[2], char *arg, char **env)
{
	pipe(out);
	return (middle_process(in, out, arg, env));
}

void	int_swap(int dest[2], int src[2])
{
	dest[0] = src[0];
	dest[1] = src[1];
}

void	c(int i)
{
	if (i != 0 && i != 1)
		close(i);
}

void	close_wait(int fd[2], int out[2], int j, int *pid)
{
	c(fd[0]);
	c(fd[1]);
	c(out[0]);
	c(out[1]);
	(void) j;
	while (wait(0) != -1)
		continue ;
	free(pid);
}

int	middle_process(int in[2], int out[2], char *args, char **env)
{
	char			**paths;
	char			*tmp;
	int				pid;
	t_command		cmd;

	pid = fork();
	if (!pid)
	{
		tmp = args;
		args = ft_strtrim(args, " 	");
		cmd = parse(args, (int[2]) {in[0], out[1]});
		free(args);
		paths = env_to_paths(env);
		dup2(cmd.fd[0], 0);
		dup2(cmd.fd[1], 1);
		if (ft_isbuiltin(cmd.command))
			execute(cmd, env);
		else
			execve(get_path(paths, cmd.command), cmd.args, env);
	}
	c(out[1]);
	return (pid);
}

void	launch_pipex(int argc, char **argv, char **env, int files[2])
{
	int	*pid;
	int	in[2];
	int	out[2];
	int	j;

	if (++argc == 0)
		argc++;
	pid = malloc(sizeof(int) * argc - 3);
	if (!pid)
		return ;
	j = 0;
	pipe(in);
	if (argc == 1)
		int_swap(in, files);
	while (j < argc)
	{
		if(j == 0)
		{
			pid[j] = middle_process(files, in, argv[j + 0], env);
			c(files[0]);
			if (argc == 2)
				int_swap(out, in);
		}
		else if ( j == argc - 1)
			pid[j] = middle_process(out, files, argv[j], env);
		else
		{
			if (j > 1)
				int_swap(in, out);
			pid[j] = launch_process(in, out, argv[j], env);
		}
		j++;
	}
	close_wait(in, out, j - 1, pid);
	free(argv);
}
