/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shalimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 15:52:46 by shalimi           #+#    #+#             */
/*   Updated: 2022/12/15 16:42:35 by shalimi          ###   ########.fr       */
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
	int				pid;
	t_command		cmd;

	pid = fork();
	if (!pid)
	{
		cmd = parse(args);
		paths = env_to_paths(env);
		dup2(in[0], 0);
		dup2(out[1], 1);
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
/*	pid[0] = first_process(files[0], in, argv[2], env);
	if (argc == 5)
		int_swap(out, in);
	j = 3;
	while (j != argc - 2)
	{
		if (j > 3)
			int_swap(in, out);
		pid[j - 2] = launch_process(in, out, argv[j], env);
		j++;
	}
	pid[j] = final_process(out, files[1], argv[argc - 2], env);*/
	close_wait(in, out, j - 1, pid);
}
/*
int	main(int argc, char **argv, char **env)
{
	int	files[2];

	if (argc < 5)
	{
		ft_putendl_fd("Mauvaise execution de la commande", 1);
		exit(1);
	}
	files[0] = open(argv[1], O_RDONLY);
	files[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (files[0] < 0 || files[1] < 0)
	{
		ft_putendl_fd("T'as fais dla merde bg du coup je quitte.", 1);
		ft_putendl_fd("Je n'arrive pas a lire l'input ou creer l'output", 1);
		ft_putendl_fd("Bisou", 1);
		ft_putendl_fd("PIPEX OVER", 1);
		exit(1);
	}
	launch_pipex(argc - 3, argv + 2, env, files);
	return (0);
}*/
