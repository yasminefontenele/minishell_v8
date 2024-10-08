/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_files.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eliskam <eliskam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 17:25:13 by emencova          #+#    #+#             */
/*   Updated: 2024/10/08 08:35:11 by eliskam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

char *read_here_document(char *input_buffer[2], size_t limit_length, char *end_marker, char *error_message)
{
    char *temp;
    size_t line_count;

    line_count = 0;
    while (g_exit_status != 130 && (input_buffer[0] == NULL
        || ft_strncmp(input_buffer[0], end_marker, limit_length) != 0 
        || ft_strlen(end_marker) != limit_length))
    {
        input_buffer[0] = readline("> ");
        if (input_buffer[0] == NULL)
        {
            printf("%s (wanted `%s`)\n", error_message, end_marker);
            break;
        }
         if (ft_strcmp(input_buffer[0], "EOF") == 0)
        {
            free(input_buffer[0]);
            return (NULL);
        }
        line_count++;
        temp = input_buffer[1];
        input_buffer[1] = ft_strjoin(input_buffer[1], input_buffer[0]);
        free(temp);
        temp = ft_strjoin(input_buffer[1], "\n");
        free(input_buffer[1]);
        input_buffer[1] = temp;
        free(input_buffer[0]);
        input_buffer[0] = NULL;
    }
    if (line_count > 0)
        printf("Here-document contains %zu lines.\n", line_count); 
    return (input_buffer[1]);
}

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

int ft_str_is_space(char *line)
{
    while (*line)
    {
        if (!ft_isspace(*line))
            return 0;
        line++;
    }
    return 1;
}



