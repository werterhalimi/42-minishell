/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shalimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 15:52:46 by shalimi           #+#    #+#             */
/*   Updated: 2022/12/29 23:23:09 by shalimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	int_swap(int dest[2], int const src[2])
{
	dest[0] = src[0];
	dest[1] = src[1];
}

void	c(int i)
{
	if (i != STDIN_FILENO && i != STDOUT_FILENO && i != STDERR_FILENO)
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
	i = -1;
//	printf("WAIT 0 : j = %d\n", j);
	while (++i < j)
	{
//		printf("WAIT 1\n");
		if (pid[i] < NO_WAIT)
			g_var.last_er = pid[i] * -1;
		else if (pid[i] != NO_WAIT)
		{
			signals();
//			printf("WAIT 2\n");
			last_pid = waitpid(pid[i], &status, 0);
			if (last_pid < 0 && errno != ECHILD) // TODO
				perror("WAIT ERROR");
			if (i == j - 1 && g_var.quit_child == NO)
			{
//				printf("ERROR 0 : %d\n", g_var.last_er);
				if (WIFEXITED(status))
				{
					g_var.last_er = WEXITSTATUS(status);
//					printf("ERROR 1 : %d\n", g_var.last_er);
				}
				if (WIFSIGNALED(status))
				{
					g_var.last_er = WTERMSIG(status) + 128;
//					printf("ERROR 2 : %d\n", g_var.last_er);
				}
				if (WIFSTOPPED(status))
				{
					g_var.last_er = WSTOPSIG(status);
//					printf("ERROR 3 : %d\n", g_var.last_er);
				}
			}
		}
//		else if (pid[i] < -1)
//			g_var.last_er = pid[i] * -1;
	}
	//while (wait(&i) != -1)
	//	continue ;
	free(pid);
//	printf("ERROR CODE : %d\n", g_var.last_er);
}

void	print_cmd(t_command cmd)
{
	printf("\n++++++++++++++++++++++++++\n");
	printf("CMD : %s\n", cmd.command);
	int	i = -1;
	while (cmd.args[++i])
		printf("%d : %s\n", i, cmd.args[i]);
	printf("%d : %s\n", i, cmd.args[i]);
	printf("fd[0] = %d\n", cmd.fd[0]);
	printf("fd[1] = %d\n", cmd.fd[1]);
	printf("++++++++++++++++++++++++++\n\n");
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
		free(cmd.args);
		return (cmd.parse_error);
	}
	if (g_var.quit_child == YES)
	{
		free(cmd.args);
		return (NO_WAIT);
	}
	//	if (argc == 1)
	if (!argc)
	{
		if (ft_isbuiltin(cmd.command))
		{
			g_var.last_er = execute(cmd);
			free(cmd.args);
			return (NO_WAIT);
		}
	}
	g_var.pid = fork();
	if (!g_var.pid)
	{
		signals();
		if (cmd.fd[0] < 0)
			exit(print_error("Ce fichier n'existe pas."));
		c(out[0]);
		paths = env_to_paths();
		dup2(cmd.fd[0], 0);
		dup2(cmd.fd[1], 1);
//		print_cmd(cmd);
		if (ft_isbuiltin(cmd.command))
			execute(cmd);
		else if (!get_path(paths, cmd.command))
			exit(PATH_ERROR);
		else
			execve(get_path(paths, cmd.command), cmd.args, g_var.envp);
		exit(0);
	}
//	if (argc != 1) ???
	if (argc)
		g_var.exit = NO;
	c(out[1]);
	free(cmd.args);
	return (g_var.pid);
}

int	launch_process(int in[2], int out[2], char *arg, int argc)
{
	pipe(out);
	return (middle_process(in, out, arg, argc));
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
		if (pid[j++] < -1)
			break;
//		j++;
	}
	close_wait(in, out, j, pid);
	free(argv);
	g_var.pid = 0;
	g_var.status = READ;
	g_var.quit_child = NO;
}
