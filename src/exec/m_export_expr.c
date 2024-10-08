/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_export_expr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eliskam <eliskam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 08:08:44 by emencova          #+#    #+#             */
/*   Updated: 2024/10/08 20:03:00 by eliskam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

char *expand_value_if_needed(t_shell *shell, char *value)
{
    if (value && value[0] == '$')
    {
        char *expanded_value = get_env_for_export(shell, value + 1);
        if (expanded_value)
        {
            free(value);
            value = ft_strdup(expanded_value);
            free(expanded_value);
        }
    }
    return (value);
}

void add_or_update_env_var(t_shell *shell, char *var_name, char *value)
{
    int index = find_key_idx(shell->keys, var_name);

    if (index != -1)
        set_env_ex(shell, var_name, value);
    else
    {
        char *new_entry = ft_strjoin(var_name, "=");
        char *full_entry = ft_strjoin(new_entry, value);
        free(new_entry);
        shell->keys = extend_form(shell->keys, full_entry);
        free(full_entry);
    }
}

void handle_export_with_value(t_shell *shell, char *arg)
{
    char *var_name;
    char *value;

    split_var_value(arg, &var_name, &value);
    value = expand_value_if_needed(shell, value);

    if (var_name && is_valid_env_var(var_name))
        add_or_update_env_var(shell, var_name, value);
    else
        write(STDERR_FILENO, "Not a valid identifier\n", 23);

    free(var_name);
    free(value);
}

void handle_export_without_value(t_shell *shell, char *arg)
{
    if (is_valid_env_var(arg))
    {
        int index = find_key_idx(shell->keys, arg);
        if (index == -1)
            shell->keys = extend_form(shell->keys, ft_strjoin(arg, "="));
    }
    else
        write(STDERR_FILENO, "Not a valid identifier\n", 23);
}

int m_export(t_shell *shell)
{
    int i;
    char **av;
    char *equals_sign;

    av = ((t_exec *)shell->cmds->content)->args;
    i = 1;
    if (!av[1])
    {
        print_export(shell->keys);
        return (0);
    }
    while (av[i])
    {
        equals_sign = ft_strchr(av[i], '=');
        if (equals_sign)
            handle_export_with_value(shell, av[i]);
        else
            handle_export_without_value(shell, av[i]);
        i++;
    }
    return (1);
}

/*
int m_export(t_shell *shell)
{
    int i;
    char **av;
    char *var_name;
    char *value;
    char *equals_sign;
    int index;

    av = ((t_exec *)shell->cmds->content)->args;
    i = 1;
    if (!av[1])
    {
        print_export(shell->keys);
        return 0;
    }
    while (av[i])
    {
        equals_sign = ft_strchr(av[i], '=');

        if (equals_sign != NULL)
        {
            split_var_value(av[i], &var_name, &value);
            if (value && value[0] == '$')
            {
                char *expanded_value = get_env_for_export(shell, value + 1);
                if (expanded_value)
                {
                    free(value);
                    value = ft_strdup(expanded_value);
                    free(expanded_value);
                }
            }  
            if (var_name && is_valid_env_var(var_name))
            {
                index = find_key_idx(shell->keys, var_name);
                if (index != -1)
                    set_env_ex(shell, var_name, value);
                else
                {
                    char *new_entry = ft_strjoin(var_name, "=");
                    char *full_entry = ft_strjoin(new_entry, value);
                    free(new_entry);
                    shell->keys = extend_form(shell->keys, full_entry);
                    free(full_entry);
                }
            }
            else
                write(STDERR_FILENO, "Not a valid identifier\n", 23);
            free(var_name);
            free(value);
            value = NULL;
        }
        else
        {
            if (is_valid_env_var(av[i]))
            {
                index = find_key_idx(shell->keys, av[i]);
                if (index == -1)
                    shell->keys = extend_form(shell->keys, ft_strjoin(av[i], "="));
            }
            else
                write(STDERR_FILENO, "Not a valid identifier\n", 23);
        }
        i++;
    }
    return 1;
}

*/