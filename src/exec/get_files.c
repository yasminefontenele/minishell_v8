/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_files.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eliskam <eliskam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 17:23:23 by emencova          #+#    #+#             */
/*   Updated: 2024/10/08 19:35:12 by eliskam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
/*
int check_file_permissions(char *path, int is_output)
{
    if (!is_output && access(path, F_OK) == -1)
        return (m_error(ERR_NEWDIR, path, 126), -1);
    if (!is_output && access(path, R_OK) == -1)
        return (m_error(ERR_NWPERM, path, 126), -1);
    if (is_output && access(path, W_OK) == -1 && access(path, F_OK) == 0)
        return (m_error(ERR_NWPERM, path, 126), -1);
    return (0);
}

int open_fd(int fd, char *path, int is_output, int append)
{
    if (fd > 2)
        close(fd);
    if (!path || check_file_permissions(path, is_output) == -1)
    {
        fprintf(stderr, "Error: Invalid path\n");
        return (-1);
    }
    if (is_output)
    {
        if (append)
            fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
        else
            fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    }
    else
        fd = open(path, O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening file");
        return (-1);
    }
    return fd;
}



int open_fd(int fd, char *path, int is_output, int append)
{
    if (fd > 2)
        close(fd); 
    if (!path)
    {
        fprintf(stderr, "Error: Invalid path\n");
        return (-1);
    }
    if (!is_output && access(path, F_OK) == -1)
    {
        m_error(ERR_NEWDIR, path, 126);
        return (-1);
    }
    if (!is_output && access(path, R_OK) == -1)
    {
        m_error(ERR_NWPERM, path, 126);
        return (-1);
    }
    if (is_output && access(path, W_OK) == -1 && access(path, F_OK) == 0)
    {
        m_error(ERR_NWPERM, path, 126);
        return (-1);
    }
    if (is_output)
    {
        if (append)
            fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
        else
            fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    }
    else
        fd = open(path, O_RDONLY);
    if (fd == -1)
        perror("Error opening file");
    return (fd);
}
*/
t_exec *outfile_one(t_exec *node, char **ags, int *len)
{
    char *error_msg;
	
	error_msg = "syntax error";
    (*len)++;
    if (ags[*len])
	{
        node->out = open_fd(node->out, ags[*len], 1, 0);
        if (node->out == -1)
		{
            ft_putendl_fd(error_msg, 2);
            g_exit_status = 1;
            *len = -1;
            return (node);
        }
    } 
	else
	{
        ft_putendl_fd(error_msg, 2);
        g_exit_status = 2;
        *len = -1;
    }
	 return (node);
}

t_exec *outfile_two(t_exec *node, char **ags, int *len) 
{
    char *new_line;
	
	new_line = "syntax error";
    (*len)++;
    if (ags[*len])
        node->out = open_fd(node->out, ags[*len], 1, 1);
    if (!ags[*len] || node->out == -1)
	{
        *len = -1;
        ft_putendl_fd(new_line, 2);
        g_exit_status = 2;
    }
    return (node);
}

t_exec	*infile_one(t_exec *node, char **args, int *i)
{
	char	*nl;
	int		flags[2];

	flags[0] = 0;
	flags[1] = 0;
	nl = "minishell: syntax error near unexpected token `newline'";
	(*i)++;
	if (args[*i])
		node->in = open_fd(node->in, args[*i], flags[0], flags[1]);
	if (!args[*i] || node->in == -1)
	{
		*i = -1;
		if (node->in != -1)
		{
			ft_putendl_fd(nl, 2);
			g_exit_status = 2;
		}
		else
			g_exit_status = 1;
	}
	return (node);
}

void infile_two(t_exec *node, char **ags, int *len)
{
    char *input[2] = {NULL, NULL};
    char *deli[2];

    //input[0] = NULL;
   // input[1] = NULL;
    deli[0] = NULL;
    deli[1] = "warning: here-document delimited by end-of-file";
    (*len)++;
    if (ags[*len])
    {
        deli[0] = ags[*len];
        node->in = create_here_document_fd(input, deli);
        if (node->in == -1)
        {
            g_exit_status = 0;
            *len = -1;
            return;
        }
        return;
    }
    if (!ags[*len] || node->in == -1)
    {
        *len = -1;
        if (node->in != -1)
            g_exit_status = 2;
    }
}







