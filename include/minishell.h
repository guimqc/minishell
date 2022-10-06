/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mraymond <mraymond@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 18:27:32 by gvial             #+#    #+#             */
/*   Updated: 2022/10/05 11:19:09 by mraymond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

//LIBRARIES=====================================================================
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <signal.h>
# include <errno.h>
# include <stdio.h>
# include <unistd.h>
# include "libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>

//==============================================================================

# ifndef DEBUG
#  define DEBUG 0
# endif

//STRUCTS=======================================================================

typedef struct s_cmd
{
	char			*cmd_path;
	char			**args;
	char			*fd_in;
	char			**fd_out;
	int				append;
	int				heredoc;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_ms
{
	char	*last_line;
	char	**envp;
	int		*pipe;
	t_cmd	*cmds;
}	t_ms;

//==============================================================================

//PROTOTYPES_FILES==============================================================
//0_main.c


//01_init.c
void	ms_init(t_ms *ms, char **envp);
t_ms	*get_ms(void);

//02_signal.c
void	signal_init(void);
void	fct_sigquit(int sig);
void	fct_sigint(int sig);
void	fct_sigabrt(int sig);

//03_parsing
int		have_sign(char *s);
void	free_lst(t_cmd *head);
t_cmd	*lst_last(t_cmd *head);
int		find_cmd_i(char **split);
void	parse(char **envp, t_ms **ms);
char	*get_cmd_path(char *cmd, char **envp);
char	**get_fd_out(t_cmd **new_cmd, char *cmd);

//04_builtins
char	**add_env_var(char **envp, char *var_name);

//05_exec.c
void	exec(t_ms **ms);

// utils

//==============================================================================

#endif