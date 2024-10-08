/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eliskam <eliskam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 22:31:57 by emencova          #+#    #+#             */
/*   Updated: 2024/10/08 19:12:19 by eliskam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include <dirent.h>


void setup_pipeline(int fd[2])
{
    if (pipe(fd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
}

void setup_redirection(int prev_fd, int fd[2], int has_next)
{
    if (prev_fd != -1)
    {
        if (dup2(prev_fd, STDIN_FILENO) == -1)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        close(prev_fd);
    }
    if (has_next)
    {
        if (dup2(fd[1], STDOUT_FILENO) == -1)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        close(fd[1]);
    }
    close(fd[0]);
}

void fork_and_execute_command(t_shell *shell, t_list *cmd_node, int prev_fd, int fd[2])
{
    pid_t pid;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0)
    {
        setup_redirection(prev_fd, fd, cmd_node->next != NULL);
        command_get_pipeline(shell, cmd_node);
        exit(EXIT_SUCCESS);
    }
    
    if (prev_fd != -1)
        close(prev_fd);
    if (cmd_node->next)
        close(fd[1]);
}

/*
void fork_and_execute_command(t_shell *shell, t_list *cmd_node, int prev_fd, int fd[2])
{
    pid_t pid;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }  
    if (pid == 0)
    {
        if (prev_fd != -1)
        {
            if (dup2(prev_fd, STDIN_FILENO) == -1)
            {
                perror("dup2");
                exit(EXIT_FAILURE);
            }
            close(prev_fd);
        }
        if (cmd_node->next)
        {
            if (dup2(fd[1], STDOUT_FILENO) == -1)
            {
                perror("dup2");
                exit(EXIT_FAILURE);
            }
            close(fd[1]);
        }
        close(fd[0]);
        command_get_pipeline(shell, cmd_node);
        exit(EXIT_SUCCESS);
    }
    else
    {
        if (prev_fd != -1)
            close(prev_fd);
        if (cmd_node->next)
        {
            close(fd[1]);
            prev_fd = fd[0];
        }
    }
}
*/


void wait_for_children()
{
    while (wait(NULL) > 0);
}

void execute_pipeline(t_shell *shell, t_list *commands_list)
{
    int fd[2];
    int prev_fd;
    t_list *cmd_node;

    cmd_node = commands_list;
    prev_fd = -1;
    while (cmd_node)
    {
        if (cmd_node->next)
            setup_pipeline(fd);
        fork_and_execute_command(shell, cmd_node, prev_fd, fd); 
        cmd_node = cmd_node->next;
    }
    wait_for_children();
}

/*
void execute_pipeline(t_shell *shell, t_list *commands_list)
{
    int fd[2];
    int prev_fd;
    t_list *cmd_node;
    
    cmd_node = commands_list;
    prev_fd = -1;
    while (cmd_node)
    {
        if (cmd_node->next)
        {
            if (pipe(fd) == -1)
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }

        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid == 0)
        {
            if (prev_fd != -1)
            {
                if (dup2(prev_fd, STDIN_FILENO) == -1)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
                close(prev_fd);
            }
            if (cmd_node->next)
            {
                if (dup2(fd[1], STDOUT_FILENO) == -1)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
                close(fd[1]);
            }
            close(fd[0]);
            command_get_pipeline(shell, cmd_node);
            exit(EXIT_SUCCESS);
        }
        else
        {
            if (prev_fd != -1)
                close(prev_fd);
            if (cmd_node->next)
            {
                close(fd[1]);
                prev_fd = fd[0];
            }
        }
        cmd_node = cmd_node->next;
    }
    while (wait(NULL) > 0);
}
*/