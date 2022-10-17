/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mraymond <mraymond@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 18:28:01 by gvial             #+#    #+#             */
/*   Updated: 2022/10/17 09:11:40 by mraymond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	exec(t_ms *ms)
{
	ms->nb_cmd = lst_len(ms->cmds);
	fd_allocation(ms);
	fd_redirection(ms);
	child_creation(ms);
	waiting_n_closefd(ms);
}

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

void	fd_redirection(t_ms *ms)
{
	int		cmd_index;
	t_cmd	*cmd;

	cmd = ms->cmds;
	cmd_index = -1;
	while (++cmd_index < ms->nb_cmd && cmd)
	{
		cmd->fildes[0] = redirection_in(cmd);
		if (cmd->fildes[0] != -1)
		{
			cmd->fildes[1] = redirection_out(cmd);
			if (cmd->fildes[0] == 0)
				cmd->fildes[0] = dup(ms->pipe[cmd_index * 2]);
			if (cmd->fildes[1] == 0)
				cmd->fildes[1] = dup(ms->pipe[(cmd_index * 2) + 1]);
		}
		cmd = cmd->next;
	}
	close_n_free_mspipe(ms);
}

void	child_creation(t_ms *ms)
{
	int		process_id;
	t_cmd	*cmd;

	cmd = ms->cmds;
	ms->cmd_index = -1;
	process_id = 1;
	while (++ms->cmd_index < ms->nb_cmd && process_id != 0 && cmd)
	{
		if (cmd->fildes[0] != -1)
			process_id = fork();
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
	int		child_id;

	close_all_cmd_fdin_fdout(ms);
	child_id = wait(0);
	while (child_id != -1)
		child_id = wait(0);
}