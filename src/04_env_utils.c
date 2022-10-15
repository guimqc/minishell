/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvial <marvin@42quebec.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 18:28:19 by gvial             #+#    #+#             */
/*   Updated: 2022/09/29 18:28:21 by gvial            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*get_varname(char *var)
{
	int		i;
	int		j;
	char	*varname;

	i = 0;
	while (var[i] && var[i] != '=')
		i++;
	varname = ft_calloc(i, sizeof(char));
	j = -1;
	while (++j < i)
		varname[j] = var[j];
	return (varname);
}

int	already_exist(char *var, char **envp)
{
	int		i;
	char	*varname;
	char	*get_var_name_;

	varname = get_varname(var);
	i = -1;
	while (envp[++i])
	{
		get_var_name_ = get_varname(envp[i]);
		if (ft_strnstr(envp[i], varname, ft_strlen(varname))
			&& ft_strlen(get_var_name_) == ft_strlen(varname))
		{
			free(get_var_name_);
			free(varname);
			return (i);
		}
		free(get_var_name_);
	}
	free(varname);
	return (-1);
}

int		is_valid_unset(char *var, int err)
{
	int	i;

	i = -1;
	while (var[++i])
	{
		if (var[i] == '=' || (i == 0 && ft_isdigit(var[i])))
		{
			if (err)
				printf("unset: `%s': not a valid identifier", var);
			return (0);
		}
	}
	return (1);
}

int	is_valid_export(char *var, int err)
{
	int		i;

	i = -1;
	while (var[++i])
	{
		if (var[i] == '=' && i != 0)
			break ;
		if (i == 0 && (!ft_isalpha(var[i]) || var[i] == '='))
		{
			if (err)
				printf("export: '%s': not a valid identifier\n", var);
			return (0);
		}
	}
	if (!ft_strnstr(var, "=", ft_strlen(var)))
		return (0);
	return (1);
}