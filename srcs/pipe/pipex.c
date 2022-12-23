/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shalimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 15:52:46 by shalimi           #+#    #+#             */
/*   Updated: 2022/12/22 00:41:57 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	middle_process(int in[3], int out[2], char *args, int argc);

int	launch_process(int in[2], int out[2], char *arg, int argc)
{
	pipe(out);
	return (middle_process(in, out, arg, argc));
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
	int	i;

	c(fd[0]);
	c(fd[1]);
	c(out[0]);
	c(out[1]);
	(void) j;
	i = 0;
	while (i < j)
	{
		if (pid[i] == -1)
		{
			i++;
			continue ;
		}
		waitpid(pid[i], &(g_var.last_er), 0);
		i++;
	}
	//while (wait(&i) != -1)
	//	continue ;
	free(pid);
}

int	middle_process(int in[2], int out[2], char *args, int argc)
{
	char			**paths;
//	int				pid;
	t_command		cmd;

	args = ft_strtrim(args, " 	");
	cmd = parse(args, (int[2]) {in[0], out[1]});
	free(args);
	if (argc == 1)
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
		else if(get_path(paths, cmd.command) == 0)
			exit(127);
		else
			execve(get_path(paths, cmd.command), cmd.args, g_var.envp);
		exit(0);
	}
	if (argc != 1)
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
	if (++argc == 0)
		argc++;
	pid = malloc(sizeof(int) * argc - 3); // TODO ????
	if (!pid)
		return ;
	j = 0;
	pipe(in);
	if(argc == 1)
		int_swap(in, files);
	while (j < argc)
	{
		if(j == 0)
		{
			pid[j] = middle_process(files, in, argv[j + 0], argc);
			c(files[0]);
			if (argc == 2)
				int_swap(out, in);
		}
		else if (j == argc - 1)
			pid[j] = middle_process(out, files, argv[j], argc);
		else
		{
			if (j > 1)
				int_swap(in, out);
			pid[j] = launch_process(in, out, argv[j], argc);
		}
		j++;
	}
	close_wait(in, out, j, pid);
	free(argv);
	g_var.pid = 0;
	g_var.status = READ;
}
