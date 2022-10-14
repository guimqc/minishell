/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   03_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mraymond <mraymond@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 18:28:28 by gvial             #+#    #+#             */
/*   Updated: 2022/10/14 12:29:04 by mraymond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	have_redirec(char *s)
{
	if (s == NULL)
		return (1);
	if (s[0] == '<' || s[0] == '>')
		return (1);
	return (0);
}

int	have_dbl_redirec(char *s)
{
	if (ft_strnstr(s, ">>", 2) || ft_strnstr(s, "<<", 2))
		return (1);
	return (0);
}

t_cmd	*lst_last(t_cmd *head)
{
	if (!head)
		return (head);
	while (head->next != NULL)
		head = head->next;
	return (head);
}

int	lst_len(t_cmd *head)
{
	int	len;

	if (!head)
		return (0);
	len = 1;
	while (head && head->next != NULL)
	{
		len++;
		head = head->next;
	}
	return (len);
}

void	free_cmds(t_ms *ms)
{
	t_cmd	*tmp;
	t_cmd	*cmd;

	cmd = ms->cmds;
	if (cmd == NULL)
		return ;
	while (cmd)
	{
		if (cmd->cmd_path)
			free(cmd->cmd_path);
		if (cmd->args)
			free_split(cmd->args);
		if (cmd->fd_in)
			free_split(cmd->fd_in);
		if (cmd->fd_out)
			free_split(cmd->fd_out);
		if (cmd->heredoc)
			free(cmd->heredoc);
		tmp = cmd;
		cmd = cmd->next;
		free(tmp);
	}
	ms->cmds = NULL;
}
