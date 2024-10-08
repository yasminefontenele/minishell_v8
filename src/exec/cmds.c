/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eliskam <eliskam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 16:34:30 by emencova          #+#    #+#             */
/*   Updated: 2024/10/08 08:35:20 by eliskam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "../../minishell.h"
#include <dirent.h>


static DIR *check_cmd(t_shell *shell, t_list *comnd, char ***str)
{
    t_exec *node;
    DIR *directory;
    char *path_env;

    directory = NULL;
    node = comnd->content;
    if (node && node->args)
        directory = opendir((const char *)node->args);
    if (node && node->args && ft_strchr(*node->args, '/') && !directory)
    {
        *str = ft_split(*node->args, '/');
        node->path = ft_strdup(*node->args);
        free(node->args[0]);
        node->args[0] = ft_strdup(str[0][form_len(*str) - 1]);
    }
    else if (!built_check(node) && node && node->args && !directory)
    {
        path_env = get_env("PATH", shell->keys, 4);
        *str = ft_split(path_env, ':');
        if (!*str)
            m_error(ERR_PIPE, "Failed to split PATH", 127);
        free(path_env);
        node->path = cmd_find(*str, *node->args, node->path);
    }
    return (directory);
}

/// LAST THAT WORKED !!
void command_get_single(t_shell *shell, t_list *comnd)
{
    t_exec *node;
    DIR *directory;
    char **str;
    pid_t pid;
    int status;
    int i;
    char *temp;

    str = NULL;
    node = comnd->content;
    if (built_check(node))
    {
        builtin(shell, comnd, &g_exit_status, ft_strlen(node->args[0]));
        return ;
    }
    i = 0;
    while (node->args[i])
    {
       temp = remove_quotes(node->args[i]);
       if (temp)
        {
            free(node->args[i]);
            node->args[i] = temp;
        }
        i++;
    }
    directory = check_cmd(shell, comnd, &str);
    if (directory)
    {
        closedir(directory);
        return ;
    }
    if (node->path && access(node->path, X_OK) == 0)
    {
        pid = fork();
        if (pid < 0)
        {
            m_error(ERR_FORK, "Fork failed", 1);
            return ;
        }
        else if (pid == 0)
        {
            execve(node->path, node->args, shell->keys);
            exit(126);
        }
        else
        {
            waitpid(pid, &status, 0);
            if (WIFEXITED(status))
                g_exit_status = WEXITSTATUS(status);
        }
    }
    else
        m_error(ERR_NEWCMD, node->args[0], 126);
    free_form(&str);
}



/// WORKS WITH unset USER | env | grep USER

void command_get_pipeline(t_shell *shell, t_list *comnd)
{
    t_exec *node;
    DIR *directory;
    char **str;

    str = NULL;
    node = comnd->content;
    if (ft_strcmp(node->args[0], "env") == 0)
    {
        m_env(shell, node->args);
        return;
    }

    if (built_check(node))
    {
        pipe_builtin(shell, comnd, &g_exit_status, ft_strlen(node->args[0]));
        return;
    }
    directory = check_cmd(shell, comnd, &str);
    if (directory)
    {
        closedir(directory);
        m_error(ERR_ISDIR, node->args[0], 126);
        return;
    }
    if (node->path && access(node->path, X_OK) == 0)
    {
        if (execve(node->path, node->args, shell->keys) == -1)
        {
            m_error(ERR_NEWCMD, node->args[0], 126);
            exit(126);
        }
    }
    else
        m_error(ERR_NEWCMD, node->args[0], 127);
    free_form(&str);
}


/// WORKS LAST!!!!!!!!!!!

void command_get_redir(t_shell *shell, t_list *comnd)
{
    t_exec *node;
    DIR *directory;
    char **str;
    pid_t pid;
    int status;
    int original_stdout;

    str = NULL;
    node = comnd->content;
    original_stdout = dup(STDOUT_FILENO); 

    if (built_check(node)) 
    {
        if (node->out != STDOUT_FILENO)
            dup2(node->out, STDOUT_FILENO);
        pipe_builtin(shell, comnd, &g_exit_status, ft_strlen(node->args[0]));
        dup2(original_stdout, STDOUT_FILENO);
        close(original_stdout);
        close(node->out);
        return;
    }
    directory = check_cmd(shell, comnd, &str);
    if (directory)
    {
        closedir(directory);
        m_error(ERR_ISDIR, node->args[0], 126);
        return;
    }
    if (node->path && access(node->path, X_OK) == 0)
    {
        pid = fork();
        if (pid < 0)
        {
            m_error(ERR_FORK, "Fork failed", 1);
            return;
        }
        else if (pid == 0)
        {
            if (node->out != STDOUT_FILENO)
                dup2(node->out, STDOUT_FILENO);
            
            directory = check_cmd(shell, comnd, &str);
            execve(node->path, node->args, shell->keys);
           // m_error(ERR_ISDIR, node->args[0], 126);
            exit(126);
        }
        else
        {
            waitpid(pid, &status, 0);
            if (WIFEXITED(status))
                g_exit_status = WEXITSTATUS(status);
            dup2(original_stdout, STDOUT_FILENO);
            close(original_stdout);
        }
    }
    else
        m_error(ERR_NEWCMD, node->args[0], 126);
    close(node->out);
}

void command_get_redir_left(t_shell *shell, t_list *comnd)
{
    t_exec *node;
    DIR *directory;
    char **str;
    pid_t pid;
    int status;
    int original_stdout;
    int original_stdin;
    int i;

    str = NULL;
    node = comnd->content;
    original_stdout = dup(STDOUT_FILENO); 
    original_stdin = dup(STDIN_FILENO);
    i = 0;
    if (built_check(node))
    {
        if (node->out != STDOUT_FILENO)
            dup2(node->out, STDOUT_FILENO);
        pipe_builtin(shell, comnd, &g_exit_status, ft_strlen(node->args[0]));
        dup2(original_stdout, STDOUT_FILENO);
        dup2(original_stdin, STDIN_FILENO);
        close(original_stdout);
        close(original_stdin);
        return;
    }
    directory = check_cmd(shell, comnd, &str);
    if (directory) {
        closedir(directory);
        m_error(ERR_ISDIR, node->args[0], 126);
        dup2(original_stdout, STDOUT_FILENO);
        dup2(original_stdin, STDIN_FILENO);
        close(original_stdout);
        close(original_stdin);
        return;
    }
    if (node->path && access(node->path, X_OK) == 0) {
        pid = fork();
        if (pid < 0)
        {
            m_error(ERR_FORK, "Fork failed", 1);
            return;
        }
        else if (pid == 0)
        {
            
            if (node->in != STDIN_FILENO)
                dup2(node->in, STDIN_FILENO);
            if (node->out != STDOUT_FILENO)
                dup2(node->out, STDOUT_FILENO);
            
            while (node->args[i])
            {
                if (ft_strcmp(node->args[i], "<") == 0 || ft_strcmp(node->args[i], ">") == 0 ||
                    ft_strcmp(node->args[i], ">>") == 0 || ft_strcmp(node->args[i], "<<") == 0)
                {
                    node->args[i] = NULL;
                }
                i++;
            } 
            execve(node->path, node->args, shell->keys);
            m_error(ERR_NEWCMD, node->args[0], 126);
            exit(126);
        }
        else
        {
            waitpid(pid, &status, 0);
            if (WIFEXITED(status))
                g_exit_status = WEXITSTATUS(status);
            dup2(original_stdout, STDOUT_FILENO);
            dup2(original_stdin, STDIN_FILENO);
        }
    } else
        m_error(ERR_NEWCMD, node->args[0], 126);
    close(node->out);
    if (node->in != STDIN_FILENO)
        close(node->in);
    
    dup2(original_stdout, STDOUT_FILENO);
    dup2(original_stdin, STDIN_FILENO);
}


void cmd_execute(t_shell *shell, t_list *commands_list)
{
    int check;
    t_exec *exec;
    
    if (!commands_list)
    {
        write(STDERR_FILENO, "Error: No commands to execute\n", 31);
        return;
    }  
    exec = commands_list->content; 
    if (is_invalid_var_assignment(exec->args[0]))
        return;     
    check = parse_redir(exec, exec->args);
    if (check == 1)
    {
        command_get_redir(shell, commands_list);
        return;
    }
    if (check == 5)
    {
        command_get_redir_left(shell, commands_list);
        return;
    }
    if (check == 2)
        return;
    else if (check == 0)
    {
        if (exec->out == -1 || exec->in == -1)
            return;
        else
            if (commands_list->next)
                execute_pipeline(shell, commands_list);
            else
                command_get_single(shell, commands_list);
    }
}
