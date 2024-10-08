/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_files.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eliskam <eliskam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 17:25:13 by emencova          #+#    #+#             */
/*   Updated: 2024/10/08 20:06:47 by eliskam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"


int create_here_document_fd(char *input_buffer[2], char *delimiter[2])
{
    int fd[2];

    g_exit_status = 0;
    if (pipe(fd) == -1)
    {
        m_perror(PIPE_READ, NULL, 1);
        return (-1);
    }
    input_buffer[1] = read_here_document(input_buffer, 0, delimiter[0], delimiter[1]);
    if (input_buffer[1] == NULL || ft_strlen(input_buffer[1]) == 0)
    {
        close(fd[PIPE_READ]);
        close(fd[PIPE_WRITE]);
        return (-1);
    }
    write(fd[PIPE_WRITE], input_buffer[1], ft_strlen(input_buffer[1]));
    free(input_buffer[1]);
    close(fd[PIPE_WRITE]);
    if (g_exit_status == 130)
    {
        close(fd[PIPE_READ]);
        return (-1);
    }
    return (fd[PIPE_READ]);
}

int parse_redir(t_exec *exec, char **args)
{
    int i;

    i = 0;
    while (args[i])
    {
        if (ft_strcmp(args[i], ">") == 0)
        {
            if (!args[i + 1])
            {
                printf("minishell: syntax error near unexpected token `newline'\n");
                exec->out = -1;
                return (0);
            }
            exec->out = open_fd(exec->out, args[i + 1], 1, 0);
            if (exec->out == -1)
            {
                perror("Error opening output file");
                return (0);
            }
            args[i] = NULL;
            args[i + 1] = NULL;
            return (1);
        }
        else if (ft_strcmp(args[i], ">>") == 0)
        {
            if (!args[i + 1])
            {
                printf("minishell: syntax error near unexpected token `newline'\n");
                exec->out = -1;
                return (0);
            }
            exec->out = open_fd(exec->out, args[i + 1], 1, 1);
            if (exec->out == -1)
            {
                perror("Error opening output file");
                return (0);
            }
            args[i] = NULL;
            args[i + 1] = NULL;
            return (1);
        }
        else if (ft_strcmp(args[i], "<") == 0)
        {
            if (!args[i + 1])
            {
                printf("minishell: syntax error near unexpected token `newline'\n");
                exec->in = -1;
                return (0);
            }
            exec = infile_one(exec, args, &i);
            if (exec->in == -1)
                return (0);
            return (5);
        }
        else if (ft_strcmp(args[i], "<<") == 0)
        {
            if (!args[i + 1])
            {
                printf("minishell: syntax error near unexpected token `newline'\n");
                exec->in = -1;
                return (0);
            }
            infile_two(exec, args, &i);
            if (exec->in == -1)
                return (0);
            return (5);
        }
        i++;
    }
    return (0);
}



