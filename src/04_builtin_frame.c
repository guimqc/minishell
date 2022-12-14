/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04_builtin_frame.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mraymond <mraymond@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 08:57:42 by mraymond          #+#    #+#             */
/*   Updated: 2022/11/07 10:26:04 by mraymond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// if cmd->args[0] is a builtin -> return 1 else return 0
int	builtin_checker(t_cmd *cmd)
{
	if (!cmd->args)
		return (0);
	if (ft_strncmp(cmd->args[0], "echo", ft_strlen("echo") + 1) == 0
		|| ft_strncmp(cmd->args[0], "cd", ft_strlen("cd") + 1) == 0
		|| ft_strncmp(cmd->args[0], "pwd", ft_strlen("pwd") + 1) == 0
		|| ft_strncmp(cmd->args[0], "export", ft_strlen("export") + 1) == 0
		|| ft_strncmp(cmd->args[0], "unset", ft_strlen("unset") + 1) == 0
		|| ft_strncmp(cmd->args[0], "env", ft_strlen("env") + 1) == 0
		|| ft_strncmp(cmd->args[0], "exit", ft_strlen("exit") + 1) == 0)
		return (1);
	return (0);
}

// Plus certain de sa necessite mais ca marche???
static void	builtin_redirection(t_ms *ms, t_cmd *cmd, int in_out)
{
	if (in_out == 0)
	{
		dup2(cmd->fildes[0], 0);
		dup2(cmd->fildes[1], 1);
	}
	else
	{
		dup2(ms->std_fd[0], 0);
		dup2(ms->std_fd[1], 1);
	}
}

// Select and execute the good builtin 
void	builtin_exec(t_ms *ms, t_cmd *cmd)
{
	builtin_redirection(ms, cmd, 0);
	if (ft_strncmp(cmd->args[0], "echo", ft_strlen("echo") + 1) == 0)
		builtin_echo(ms, cmd);
	else if (ft_strncmp(cmd->args[0], "cd", ft_strlen("cd") + 1) == 0)
		builtin_cd(ms, cmd);
	else if (ft_strncmp(cmd->args[0], "pwd", ft_strlen("pwd") + 1) == 0)
		builtin_pwd(ms, cmd);
	else if (ft_strncmp(cmd->args[0], "export", ft_strlen("export") + 1) == 0)
		ms->envp = export_env_var(cmd->args, ms);
	else if (ft_strncmp(cmd->args[0], "unset", ft_strlen("unset") + 1) == 0)
		ms->envp = unset_env_var(cmd->args, ms);
	else if (ft_strncmp(cmd->args[0], "env", ft_strlen("env") + 1) == 0)
		print_split(ms->envp);
	else if (ft_strncmp(cmd->args[0], "exit", ft_strlen("exit") + 1) == 0)
		builtin_exit(ms, cmd);
	builtin_redirection(ms, cmd, 1);
}
