/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_cd_pwd_echo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eliskam <eliskam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:15:09 by emencova          #+#    #+#             */
/*   Updated: 2024/10/08 19:42:23 by eliskam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int is_no_newline_option(char *arg)
{
    if (!ft_strncmp(arg, "-n", 2) && char_count(arg, 'n') == (int)(ft_strlen(arg) - 1))
        return (1);
    return (0);
}

void print_echo_args(char **args, int start_idx, int add_space)
{
    int i;

    i = start_idx;
    while (args[i])
    {
        ft_putstr_fd(args[i], STDOUT_FILENO);
        if (args[i + 1])
            ft_putchar_fd(' ', STDOUT_FILENO);
        i++;
    }
    if (add_space)
        ft_putchar_fd('\n', STDOUT_FILENO);
}

int m_echo(char **args)
{
    int new_line;
    int i;

    new_line = 1;
    i = 1;
    if (!args || !args[0])
        return (0);
    while (args[i] && is_no_newline_option(args[i]))
    {
        new_line = 0;
        i++;
    }
    print_echo_args(args, i, new_line);
    return (0);
}


/*
int m_cd(t_shell *shell)
{
    char    **args;
    char    *home_dir;
    char    *current_dir;
    char    *target_dir;

   g_exit_status = 0;
    args = ((t_exec *)shell->cmds->content)->args;
    if (args[1] && args[2])
    {
        ft_putstr_fd("cd: too many arguments\n", 2);
        g_exit_status = 1;
        return (g_exit_status);
    }
    home_dir = get_env("HOME", shell->keys, 4);
    if (!home_dir)
    {
        ft_putstr_fd("minishell: cd: HOME not set\n", 2);
        g_exit_status = 1;
        return (g_exit_status);
    }
    if (!args[1] || ft_strcmp(args[1], "") == 0)
        target_dir = ft_strdup(home_dir);
    else if (ft_strcmp(args[1], "-") == 0)
    {
        target_dir = get_env("OLDPWD", shell->keys, 6);
        if (!target_dir)
        {
            ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
            g_exit_status = 1;
            free(home_dir);
            return (g_exit_status);
        }
    }
    else
        target_dir = ft_strdup(args[1]);
    error_cd(args, target_dir);
    if (!g_exit_status)
    {
        current_dir = getcwd(NULL, 1024);
        if (current_dir)
        {
            shell->keys = set_env("OLDPWD", current_dir, shell->keys, 6);
            shell->keys = set_env("PWD", current_dir, shell->keys, 3);
            free(current_dir);
        }
    }

    free(home_dir);
    free(target_dir);
    return (g_exit_status);
}



int m_echo(char **args)
{
    int new_line;
    int valid_arg;
    int i;

    new_line = 1;
    valid_arg = 0;
    i = 1;
    if (!args || !args[0])
        return (0);
    if (!args[1])
    {
        ft_putchar_fd('\n', STDOUT_FILENO);
        return (0);
    }
    while (args[i])
    {
        if (!valid_arg && !ft_strncmp(args[i], "-n", 2) &&
            (char_count(args[i], 'n') == (int)(ft_strlen(args[i]) - 1)))
            new_line = 0;
        else
        {
            valid_arg = 1;
            ft_putstr_fd(args[i], STDOUT_FILENO);
            if (args[i + 1])
                ft_putchar_fd(' ', STDOUT_FILENO);
        }
        i++;
    }
    if (valid_arg && new_line)
        ft_putchar_fd('\n', STDOUT_FILENO);
    return (0);
}
*/
