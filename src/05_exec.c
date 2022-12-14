/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mraymond <mraymond@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 18:28:01 by gvial             #+#    #+#             */
/*   Updated: 2022/11/07 11:13:13 by mraymond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* Set nb_cmd and malloc child_id
** set the signal to execution mode
** if just one cmd and its a built in, execute builtin in main process
** else create process for each process*/
void	exec(t_ms *ms)
{
	ms->nb_cmd = lst_len(ms->cmds);
	ms->child_id = (int *)ft_calloc(ms->nb_cmd, sizeof(int));
	signal_init(1);
	fd_allocation(ms);
	fd_redirection(ms);
	if (ms->signal == 0)
	{
		if (ms->nb_cmd == 1 && ms->cmds->fildes[0] != -1
			&& ms->cmds->fildes[1] != -1
			&& builtin_checker(ms->cmds) == 1)
			builtin_exec(ms, ms->cmds);
		else
		{
			child_creation(ms);
			waiting_n_closefd(ms);
		}
	}
}

// Create pipe for link between cmd and fill ms->pipe
void	fd_allocation(t_ms *ms)
{
	int	i;
	int	temp_pipe[2];

	ms->pipe = (int *)malloc(sizeof(int) * ((ms->nb_cmd - 1) * 2 + 2));
	ms->pipe[0] = 0;
	ms->pipe[(ms->nb_cmd - 1) * 2 + 1] = 1;
	i = -1;
	while (++i < ms->nb_cmd - 1)
	{
		pipe(temp_pipe);
		ms->pipe[i * 2 + 1] = temp_pipe[1];
		ms->pipe[i * 2 + 2] = temp_pipe[0];
	}
}

//Fill cmd->fildes with redirection or ms->pipe. Free ms-pipe
void	fd_redirection(t_ms *ms)
{
	int		cmd_index;
	t_cmd	*cmd;

	cmd = ms->cmds;
	cmd_index = -1;
	while (++cmd_index < ms->nb_cmd && cmd)
	{
		cmd->fildes[0] = redirection_in(ms, cmd);
		if (cmd->fildes[0] != -1)
		{
			cmd->fildes[1] = redirection_out(cmd);
			if (cmd->fildes[1] != -1)
			{
				if (cmd->fildes[0] == 0)
					cmd->fildes[0] = dup(ms->pipe[cmd_index * 2]);
				if (cmd->fildes[1] == 0)
					cmd->fildes[1] = dup(ms->pipe[(cmd_index * 2) + 1]);
			}
		}
		if (cmd->fildes[0] == -1 || cmd->fildes[1] == -1)
			ms->err_last_child = 1;
		cmd = cmd->next;
	}
	close_n_free_mspipe(ms);
}

// Loop of child process + fill ms->child_id with process id 
void	child_creation(t_ms *ms)
{
	int		process_id;
	t_cmd	*cmd;

	cmd = ms->cmds;
	ms->cmd_index = -1;
	process_id = 1;
	while (++ms->cmd_index < ms->nb_cmd && process_id != 0 && cmd)
	{
		if (cmd->fildes[0] != -1 && cmd->fildes[1] != -1)
		{
			process_id = fork();
			if (process_id != 0)
				ms->child_id[ms->cmd_index] = process_id;
		}
		else
			ms->skip_cmd += 1;
		cmd = cmd->next;
	}
	if (process_id == 0)
	{
		ms->cmd_index -= 1;
		child_execution(ms);
	}
}

/* close all fd
wait all child finish
*/
void	waiting_n_closefd(t_ms *ms)
{
	int	i;
	int	child_id;
	int	status;

	close_all_cmd_fdin_fdout(ms);
	i = -1;
	while (++i < ms->nb_cmd - ms->skip_cmd)
	{
		child_id = waitpid(0, &status, 0);
		if (child_id != -1)
		{
			if (child_process_to_index(ms, child_id) == ms->nb_cmd - 1)
				ms->err_last_child = WEXITSTATUS(status);
		}
		else
			ms->err_last_child = WEXITSTATUS(status);
	}
	if_xnot0_skipline();
	if (ms->signal != 0 && ms->nb_cmd == 1)
		ms->err_last_child = 130;
}
