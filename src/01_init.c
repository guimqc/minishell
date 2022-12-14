/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   01_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mraymond <mraymond@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 09:28:21 by mraymond          #+#    #+#             */
/*   Updated: 2022/11/07 09:18:35 by mraymond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Set the line prompter message (ms-> actual file $>)
void	fill_line_prompter(t_ms *ms)
{
	int		i_last_dir;
	char	path[1000];

	ft_memset(ms->line_prompt, '\0', 200);
	ft_strlcpy(ms->line_prompt, PROMPTER_TITLE, ft_strlen(PROMPTER_TITLE) + 1);
	ms->line_path = &ms->line_prompt[ft_strlen(PROMPTER_TITLE)];
	getcwd(path, 1000);
	i_last_dir = ft_strrchr_i(path, '/') + 1;
	ft_strlcpy(ms->line_path, &path[i_last_dir],
		ft_strlen(&path[i_last_dir]) + 1);
	ft_strlcpy(&ms->line_path[ft_strlen(&path[i_last_dir])],
		PROMPTER_END, ft_strlen(PROMPTER_END) + 1);
}

//Initialize all the ms values
void	ms_init(t_ms *ms, char **envp)
{
	char	*shlvl;
	char	*shlvl_up;

	ms->envp = splitdup(envp);
	shlvl = get_varvalue(ms->envp[already_exist(ft_strdup("SHLVL"), ms->envp)]);
	shlvl_up = ft_itoa(ft_atoi(shlvl) + 1);
	ms->envp = export_("SHLVL=", shlvl_up, ms);
	ms->envp = export_("SHELL=", "minishell", ms);
	free(shlvl);
	free(shlvl_up);
	ms->cmds = NULL;
	ms->last_line = NULL;
	ms->line_path = NULL;
	ms->pipe = NULL;
	ms->cmds = NULL;
	ms->child_id = NULL;
	ms->cmd_index = 0;
	ms->nb_cmd = 0;
	ms->err_last_child = 0;
	fill_line_prompter(ms);
	ms->std_fd[0] = dup(0);
	ms->std_fd[1] = dup(1);
}

// free and reset all the ms->values for next line commands
void	ms_reset(t_ms *ms)
{
	if (ms->last_line)
		free(ms->last_line);
	ms->last_line = NULL;
	if (ms->cmds)
		free_cmds(ms);
	ms->cmds = NULL;
	if (ms->pipe)
		close_n_free_mspipe(ms);
	ms->pipe = NULL;
	if (ms->child_id)
		free(ms->child_id);
	ms->child_id = NULL;
	fill_line_prompter(ms);
	ms->cmd_index = 0;
	ms->nb_cmd = 0;
	ms->skip_cmd = 0;
}

/* Malloc and/or return or free ms
** put 1 in arg to free*/
t_ms	*get_ms(int erase)
{
	static t_ms	*ms = NULL;

	if (!ms)
		ms = (t_ms *)malloc(sizeof(t_ms));
	if (erase == 1)
	{
		free(ms);
		ms = NULL;
	}
	return (ms);
}
