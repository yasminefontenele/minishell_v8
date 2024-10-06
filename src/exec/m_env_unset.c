/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_env_unset.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eliskam <eliskam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:15:49 by emencova          #+#    #+#             */
/*   Updated: 2024/10/04 16:58:31 by eliskam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "../../minishell.h"

char *ft_strndup(char *src, int n)
{
    char *dest;
    int len;
    int i;

    len = 0;
    i = 0;
    while (src[len] && len < n)
        len++;
    dest = (char *)malloc(len + 1);
    if (!dest)
        return (NULL);

    while (i < len)
    {
        dest[i] = src[i];
        i++;
    }
    dest[len] = '\0';
    return (dest);
}

int find_key_idx(char **keys,char *key)
{
    int i;
    int key_len;
    char *equal_sign;
    
    i = 0;
    equal_sign = ft_strchr(key, '=');
    if (equal_sign)
        key_len = equal_sign - key;
    else
        key_len = ft_strlen(key);

    while (keys[i])
    {
        if (!ft_strncmp(keys[i], key, key_len) && keys[i][key_len] == '=')
            return (i);
        i++;
    }

    return (-1);
}

int m_unset(t_shell *shell)
{
    char **av;
	char *var_env;
    int i;
    int index;

	i = 1;
	av = ((t_exec *)shell->cmds->content)->args;
    if (form_len(av) < 2)
        return (0);
    if (av[1][0] == '-' && av[1][1] == 'n')
        return (0);
    while (av[i])
    {
        if (av[i][ft_strlen(av[i]) - 1] != '=')
        {
            var_env = ft_strjoin(av[i], "=");
            free(av[i]);
            av[i] = var_env;
        }
        index = find_key_idx(shell->keys, av[i]);
        if (index != -1)
        {
            free(shell->keys[index]);
            while (shell->keys[index])
            {
                shell->keys[index] = shell->keys[index + 1];
                index++;
            }
        }
        i++;
    }
    return (0);
}
/*
int m_unset(t_shell *shell)
{
    char **av;
    char *var_env;
    int i;
    int index;

    i = 1;
    av = ((t_exec *)shell->cmds->content)->args;

    // Check if the command is "unset ALL VARIABLES"
    if (av[1] && ft_strcmp(av[1], "ALL") == 0 && av[2] && ft_strcmp(av[2], "VARIABLES") == 0)
    {
        i = 0;
        while (shell->keys[i])
        {
            free(shell->keys[i]);
            i++;
        }
        free(shell->keys);
        shell->keys = NULL;
        return (0);
    }
    if (form_len(av) < 2)
        return (0);
    while (av[i])
    {
        if (av[i][ft_strlen(av[i]) - 1] != '=')
        {
            var_env = ft_strjoin(av[i], "=");
            free(av[i]);
            av[i] = var_env;
        }
        index = find_key_idx(shell->keys, av[i]);
        if (index != -1)
        {
            free(shell->keys[index]);
            while (shell->keys[index])
            {
                shell->keys[index] = shell->keys[index + 1];
                index++;
            }
        }
        i++;
    }
    return (0);
}


int m_env(t_shell *shell, char **args)
{
    int i;

    if (!shell || !shell->keys)
    {
        ft_error("No environment is set", 1);
        return (1);
    }
    if (args && args[1] && ft_strcmp(args[1], "PATH") == 0)
    {
        ft_error("env:'PATH': No such file or directory ", 127);
        return (1);
    }
    i = 0;
    while (shell->keys[i])
    {
        printf("%s\n", shell->keys[i]);
        i++;
    }
    
    return (0);
}
*/

int m_env(t_shell *shell, char **args)
{
    int i;
    char *equals_sign ;

    if (!shell || !shell->keys)
    {
        ft_error("No environment is set", 1);
        return (1);
    }
    if (args && args[1] && ft_strcmp(args[1], "PATH") == 0)
    {
        ft_error("env:'PATH': No such file or directory ", 127);
        return (1);
    }
    i = 0;
    while (shell->keys[i])
    {
        equals_sign = ft_strchr(shell->keys[i], '=');
        if (equals_sign && equals_sign[1] != '\0')
        {
            printf("%s\n", shell->keys[i]);
        }
        i++;
    } 
    return (0);
}
