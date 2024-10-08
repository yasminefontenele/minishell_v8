//HEADER FILE


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

void duplicate_file_descriptors(int *original_stdout, int *original_stdin)
{
    *original_stdout = dup(STDOUT_FILENO);
    *original_stdin = dup(STDIN_FILENO);
}

void restore_file_descriptors(int original_stdout, int original_stdin)
{
    dup2(original_stdout, STDOUT_FILENO);
    dup2(original_stdin, STDIN_FILENO);
    close(original_stdout);
    close(original_stdin);
}

int handle_builtin_commands(t_shell *shell, t_exec *node, t_list *comnd)
{
    if (built_check(node))
    {
        if (node->out != STDOUT_FILENO)
            dup2(node->out, STDOUT_FILENO);
        pipe_builtin(shell, comnd, &g_exit_status, ft_strlen(node->args[0]));
        return 1; // Indicate that a built-in command was handled
    }
    return 0; // Indicate that it was not a built-in command
}

int check_directory(t_shell *shell, t_list *comnd, char ***str, t_exec *node)
{
    DIR *directory = check_cmd(shell, comnd, str);
    if (directory)
    {
        closedir(directory);
        m_error(ERR_ISDIR, node->args[0], 126);
        return 1; // Indicate that a directory error occurred
    }
    return 0; // No directory error
}

void cleanup_redirection_tokens(t_exec *node)
{
    int i = 0;
    while (node->args[i])
    {
        if (ft_strcmp(node->args[i], "<") == 0 || 
            ft_strcmp(node->args[i], ">") == 0 ||
            ft_strcmp(node->args[i], ">>") == 0 || 
            ft_strcmp(node->args[i], "<<") == 0)
        {
            node->args[i] = NULL; // Nullify the redirection tokens
        }
        i++;
    }
}

void fork_and_execute(t_shell *shell, t_exec *node)
{
    pid_t pid = fork();
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

        execve(node->path, node->args, shell->keys);
        m_error(ERR_NEWCMD, node->args[0], 126);
        exit(126); // Exit if execve fails
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            g_exit_status = WEXITSTATUS(status);
    }
}

void command_get_redir_left(t_shell *shell, t_list *comnd)
{
    t_exec *node = comnd->content;
    int original_stdout, original_stdin;
    char **str = NULL;

    duplicate_file_descriptors(&original_stdout, &original_stdin);

    if (handle_builtin_commands(shell, node, comnd))
    {
        restore_file_descriptors(original_stdout, original_stdin);
        return;
    }

    if (check_directory(shell, comnd, &str, node))
    {
        restore_file_descriptors(original_stdout, original_stdin);
        return;
    }

    if (node->path && access(node->path, X_OK) == 0)
    {
        cleanup_redirection_tokens(node); // Clean up redirection tokens
        fork_and_execute(shell, node); // Fork and execute the command
    }
    else
    {
        m_error(ERR_NEWCMD, node->args[0], 126);
    }

    close(node->out);
    if (node->in != STDIN_FILENO)
        close(node->in);

    restore_file_descriptors(original_stdout, original_stdin);
}

/*
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
    if (directory)
    {
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
}*/