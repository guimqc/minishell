/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mraymond <mraymond@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 18:27:32 by gvial             #+#    #+#             */
/*   Updated: 2022/11/07 10:49:19 by mraymond         ###   ########.fr       */
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
# include "readline.h"
# include "history.h"
# include <termios.h>
# include <sys/stat.h>
# include <string.h>

//==============================================================================

# ifndef DEBUG
#  define DEBUG 0
# endif

//MESSAGES=====================================================================

//prompter
# define PROMPTER_TITLE " ms-> "
# define PROMPTER_END " $> "

# define ERR_FIRST "minishell: "

//parsing
# define GREATER_THAN ">>"
# define LESSER_THAN "<<"

//line error
# define ERR_LINE_QUOTE "line parse error -> open quote"
# define ERR_LINE_NOCMD "line parse error -> no command"
# define ERR_SYNTAX_ERROR "syntax error near unexpected token "
# define ERR_LINE_PIPE "`|'"
# define ERR_LINE_4IN "`<'"
# define ERR_LINE_5IN "`<<'"
# define ERR_LINE_3OUT "`>'"
# define ERR_LINE_4OUT "`>>'"
# define ERR_LINE_NEWLINE "`newline'"

//access_err
# define ERR_OPEN_NOSUCH ": No such file or directory"
# define ERR_OPEN_PERM ": Permission denied"
# define ERR_OPEN_ISDIR ": Is a directory"

//execve err
# define ERR_EXECVE "command not found: "

//builtin error
# define ERR_BUILT_TOOMANYARGS ": too many arguments"
# define ERR_BUILT_NOTINPWD ": string not in pwd: "
# define ERR_BUILT_PDFRULES ": not covered according to subject"
# define ERR_BUILT_NONNUM ": numeric argument required"
# define ERR_BUILT_NOTDIR ": Not a directory"

//==============================================================================

//ENUM_ERROR====================================================================

enum e_line_err
{
	lineerr_quote = 1,
	lineerr_nocmd,
	lineerr_pipe,
	lineerr_3in,
	lineerr_4in,
	lineerr_3out,
	lineerr_4out
};

enum e_open_err
{
	openerr_nosuch = 1,
	openerr_perm,
	openerr_isdir
};

//==============================================================================

//ENUM_SIGNAL

enum e_signal_sit
{
	sit_prompter,
	sit_exec,
	sit_here_doc
};

//STRUCTS=======================================================================

typedef struct s_cmd
{
	char			*cmd_path;
	char			**args;
	char			**fd_in;
	char			**fd_out;
	int				append;
	int				*heredoc;
	struct s_cmd	*next;
	int				fildes[2];
}	t_cmd;

typedef struct s_ms
{
	char			*last_line;
	char			**envp;
	int				*pipe;
	t_cmd			*cmds;
	int				*child_id;
	char			line_prompt[200];
	char			*line_path;
	int				cmd_index;
	int				nb_cmd;
	int				err_last_child;
	int				signal;
	int				skip_cmd;
	int				std_fd[2];
	int				here_doc_id;
}	t_ms;

//==============================================================================

//PROTOTYPES_FILES==============================================================

//0_main.c
void	history_clear_n_exit(t_ms *ms);

//01_init.c
void	fill_line_prompter(t_ms *ms);
void	ms_init(t_ms *ms, char **envp);
void	ms_reset(t_ms *ms);
t_ms	*get_ms(int erase);

//01_valid_line
int		valid_line(char *line);
int		print_line_err(int error);
int		valid_line_error_conversion(int valid_line_return);

//02_signal.c
void	signal_init(int situation);
void	fct_sigquit(int sig);
void	fct_sigint(int sig);
void	fct_sigint_prompter(int sig);
void	fct_sigint_here_doc(int sig);

//03_parsing
void	parse(t_ms *ms);
int		is_pipe(char *s);
void	free_cmds(t_ms *ms);
int		new_i(char *cmd, int i);
int		have_redirec(char *s);
int		have_dbl_redirec(char *s);
char	**split_cmds(char **cmd);
char	**split_quotes(char *cmd);
char	*remove_quotes(char *s);
int		find_cmd_i(char **split);
char	*isolate_cmd(char *cmd_brut);
char	*format_line(char *last_line);
char	*get_cmd_path(char **cmd, char **envp);
char	**get_fds(t_cmd *new_cmd, char **cmd, char sign);
void	conv_env_var(char **cmd, t_ms *ms, int here_doc);

//BUILTINS----------------------------------------------------------------------
//04_builtin_frame.c
int		builtin_checker(t_cmd *cmd);
void	builtin_exec(t_ms *ms, t_cmd *cmd);

//04_pwd.c
void	builtin_pwd(t_ms *ms, t_cmd *cmd);

//04_cd.c
void	builtin_cd(t_ms *ms, t_cmd *cmd);

//04_echo.c
void	builtin_echo(t_ms *ms, t_cmd *cmd);

//04_exit.c
void	builtin_exit(t_ms *ms, t_cmd *cmd);

//04_env
char	**export_env_var(char **args, t_ms *ms);
char	**unset_env_var(char **args, t_ms *ms);
void	export_print(char **envp);
char	*get_varvalue(char *var);
char	*get_varname(char *var);
int		valid_unset(char *var, int err);
int		valid_export(char *var, int err);
int		already_exist(char *varname, char **envp);
char	**export_(char *varname, char *varvalue, t_ms *ms);

//utils2
//void	free_dbl_ptr(void **ptr, int option);
void	close_all_cmd_fdin_fdout(t_ms *ms);
t_cmd	*lst_last(t_cmd *head);
int		lst_len(t_cmd *head);

//EXECUTION---------------------------------------------------------------------
//05_exec.c
void	exec(t_ms *ms);
void	fd_allocation(t_ms *ms);
void	fd_redirection(t_ms *ms);
void	child_creation(t_ms *ms);
void	waiting_n_closefd(t_ms *ms);

//05_child_exec.c
void	child_execution(t_ms *ms);
int		pipe_redirection(t_ms *ms, t_cmd *cmd);
void	child_exit(t_ms *ms);
void	child_cleaning(t_ms *ms);

//05_redirection.c
int		redirection_in(t_ms *ms, t_cmd *cmd);
int		open_fd_in(char *filename);
int		print_open_err(char *filename, int error);
int		am_i_this(char *path, char f_or_d, int r, int w);
int		redirection_out(t_cmd *cmd);

//05_here_doc.c
int		here_doc(t_ms *ms, char *str_eof);

//05_exec_utils.c
void	close_n_free_mspipe(t_ms *ms);
void	closefd_ifopen(int fd);
t_cmd	*cmd_lst_index(t_ms *ms, int cmd_index);
int		child_process_to_index(t_ms *ms, int waitpid_return);
void	if_xnot0_skipline(void);

//==============================================================================

#endif
