/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mraymond <mraymond@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 18:27:46 by gvial             #+#    #+#             */
/*   Updated: 2022/11/07 09:13:46 by mraymond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* reset signal et ms
** gestion du ctrl d et champs vide
** add_history
** return if a valid line*/
static int	prompter(t_ms *ms)
{
	ms->signal = 0;
	ms_reset(ms);
	signal_init(0);
	ms->last_line = readline(ms->line_prompt);
	while (ms->last_line && (ft_strlen(ms->last_line) == 0
			|| ft_isallspace(ms->last_line)))
	{
		ms_reset(ms);
		ms->last_line = readline(ms->line_prompt);
	}
	if (ms->last_line == NULL)
		history_clear_n_exit(ms);
	ms->signal = 0;
	add_history(ms->last_line);
	return (valid_line(ms->last_line));
}

int	main(int ac, char **av, char **envp)
{
	t_ms	*ms;
	int		line_err;

	(void) ac;
	(void) av;
	ms = get_ms(0);
	ms_init(ms, envp);
	while (1)
	{
		line_err = prompter(ms);
		if (!line_err)
		{
			parse(ms);
			exec(ms);
			free_cmds(ms);
		}
		else
			ms->err_last_child = valid_line_error_conversion(line_err);
	}
	history_clear_n_exit(ms);
	return (0);
}

/* Keep exit status
** Write exit
**clean and close everything
** exit status*/
void	history_clear_n_exit(t_ms *ms)
{
	int	status;

	status = ms->err_last_child;
	write(1, "exit\n", 5);
	close_all_cmd_fdin_fdout(ms);
	closefd_ifopen(ms->std_fd[0]);
	closefd_ifopen(ms->std_fd[1]);
	ms_reset(ms);
	if (ms->envp)
		free_split(ms->envp);
	get_ms(1);
	clear_history();
	exit(status);
}
