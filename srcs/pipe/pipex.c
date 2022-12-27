/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shalimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 15:52:46 by shalimi           #+#    #+#             */
/*   Updated: 2022/12/27 22:43:35 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	middle_process(int in[3], int out[2], char *args, int argc);

int	launch_process(int in[2], int out[2], char *arg, int argc)
{
	pipe(out);
	return (middle_process(in, out, arg, argc));
}

void	int_swap(int dest[2], int const src[2])
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
	int	i;
	int	status;
	int	last_pid;

	c(fd[0]);
	c(fd[1]);
	c(out[0]);
	c(out[1]);
//	(void) j;
	i = -1;
	while (++i < j)
	{
		if (pid[i] != -1)
		{
			signals();
			last_pid = waitpid(pid[i], &status, 0);
			if (last_pid < 0 && errno != ECHILD) // TODO
				perror("WAIT ERROR");
			if (i == j - 1)
			{
				if (WIFEXITED(status))
					g_var.last_er = status;
				else if (WIFSIGNALED(status))
					g_var.last_er = WTERMSIG(status) + 128;
				else if (WIFSTOPPED(status))
					g_var.last_er = WSTOPSIG(status);
			}
		}
		else if (pid[i] < -1)
			g_var.last_er = pid[i] * -1;

	}
	//while (wait(&i) != -1)
	//	continue ;
	free(pid);
}

int	middle_process(int in[2], int out[2], char *args, int argc)
{
	char		**paths;
	t_command	cmd;
//	int			pid;

	args = ft_strtrim(args, " 	");
	cmd = parse(args, (int[2]) {in[0], out[1]});
	free(args);
	if (cmd.parse_error)
	{
		//free(cmd);
		return (cmd.parse_error);
	}
	if (g_var.exec == ERROR)
		return (-1);
	//	if (argc == 1)
	if (!argc)
	{
		if (ft_isbuiltin(cmd.command))
		{
			execute(cmd);
			//TODO FREE COMMAND
			return (-1);
		}
	}
	g_var.pid = fork();
	if (!g_var.pid)
	{
		signals();
		if (cmd.fd[0] < 0)
		{
			printf("Ce fichier n'existe pas.\n");
			exit(1);
		}
		c(out[0]);
		paths = env_to_paths();
		dup2(cmd.fd[0], 0);
		dup2(cmd.fd[1], 1);
		if (ft_isbuiltin(cmd.command))
			execute(cmd);
		else if (get_path(paths, cmd.command) == 0)
			exit(127);
		else
			execve(get_path(paths, cmd.command), cmd.args, g_var.envp);
		exit(0);
	}
//	if (argc != 1) ???
	if (argc)
		g_var.exit = 0;
	c(out[1]);
	return (g_var.pid);
}

void	launch_pipex(int argc, char **argv, int files[2])
{
	int	*pid;
	int	in[2];
	int	out[2];
	int	j;

	g_var.status = EXECUTE;
//	if (++argc == 0)
//		argc++;
//	pid = malloc(sizeof(int) * argc - 3); // TODO ????
	pid = malloc(sizeof(int) * (argc + 1));
	if (!pid)
		return ;
	j = 0;
	pipe(in);
//	if(argc == 1)
	if (!argc)
		int_swap(in, files);
//	while (j < argc)
	while (j <= argc)
	{
		if(j == 0)
		{
			pid[j] = middle_process(files, in, argv[j + 0], argc);
			c(files[0]);
//			if (argc == 2)
			if (argc == 1)
				int_swap(out, in);
		}
//		else if (j == argc - 1)
		else if (j == argc)
			pid[j] = middle_process(out, files, argv[j], argc);
		else
		{
			if (j > 1) // ???
				int_swap(in, out);
			pid[j] = launch_process(in, out, argv[j], argc);
		}
		if (pid[j] < -1)
			break;
		j++;
	}
	close_wait(in, out, j, pid);
	free(argv);
	g_var.pid = 0;
	g_var.status = READ;
	g_var.exec = SUCCESS;
}
