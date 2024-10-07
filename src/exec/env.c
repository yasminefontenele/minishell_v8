/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:16:06 by emencova          #+#    #+#             */
/*   Updated: 2024/10/07 20:09:04 by yfontene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"


char	**set_env(char *str, char *val, char **keys, int nbr)
{
	int		i;
	int		key_len;
	char	*new_entry;
	char	*full_entry;
	
	if (nbr < 0)
		nbr = ft_strlen(str);	
	full_entry = ft_strjoin(str, "=");
	new_entry = ft_strjoin(full_entry, val);
	free(full_entry);
	i = 0;
	while (keys && keys[i])
	{
		key_len = second_strchr(keys[i], '=');
		if (ft_strncmp(keys[i], str, nbr) == 0 && (key_len >= nbr))
		{
			free(keys[i]);
			keys[i] = new_entry;
			return (keys);
		}
		i++;
	}
	keys = extend_form(keys, new_entry);
	return (keys);
}


void set_env_ex(t_shell *shell, char *var_name, char *value)
{
    int index;
    char *new_entry;
	char *tmp;

    tmp = ft_strjoin(var_name, "=");
	if(!tmp)
		ft_error("Malloc failed in set_env_ex", 1);
    new_entry = ft_strjoin(tmp, value);
	free(tmp);
    index = find_key_idx(shell->keys, var_name);
    if (index != -1)
    {
        free(shell->keys[index]);
        shell->keys[index] = ft_strdup(new_entry);
    }
    else
        shell->keys = extend_form(shell->keys, new_entry);
    free(new_entry);
}

char *get_env_for_export(t_shell *shell, const char *var)
{
    int i;
    char *env_var;
    char *env_value;

	i = 0;
    if (!var || !*var)
        return NULL;
    while (shell->keys[i])
    {
        env_var = ft_strjoin(var, "=");
        if (ft_strncmp(shell->keys[i], env_var, ft_strlen(env_var)) == 0)
        {
            env_value = shell->keys[i] + ft_strlen(env_var);
            free(env_var);
            return ft_strdup(env_value);
        }
        free(env_var);
		i++;
    }
    return (NULL);
}


char	*get_env(char *name, char **env, int name_len)
{
	int	i;
	int	n;

	i = 0;
	if (name_len < 0)
		name_len = ft_strlen(name);
	while (!ft_strchr(name, '=') && env && env[i])
	{
		n = name_len;
		if (n < second_strchr(env[i], '='))
			n = second_strchr(env[i], '=');
		if (!ft_strncmp(env[i], name, n))
			return (ft_substr(env[i], n + 1, ft_strlen(env[i])));
		i++;
	}
	return (NULL);
}