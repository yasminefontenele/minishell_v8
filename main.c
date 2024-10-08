/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eliskam <eliskam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 17:01:48 by emencova          #+#    #+#             */
/*   Updated: 2024/10/08 18:19:12 by eliskam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "./src/exec/execute.h"

t_env   g_env;

void	ft_free(void *content)
{
	t_exec *node;
    
    node = content;
	free_form(&node->args);
	free(node->path);
	if (node->in != STDIN_FILENO)
		close(node->in);
	if (node->out != STDOUT_FILENO)
		close(node->out);
	free(node);
}

void	sigint_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_env.exit_status = 130;
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_replace_line("", 0);
		rl_on_new_line();
	}
}

int handle_tab()
{
    return 0;
}
/*
/// TRYING NEW ONE THIS ONE IS ORIGINAL
void cleanup_shell(t_shell *shell, t_exec *exec, t_tokens *tokens)
{
    if (shell->cmds)
    {
        ft_lstclear(&(shell->cmds), ft_free);
        shell->cmds = NULL;
    }
    if (shell->keys)
    {
        int i = 0;
        while (shell->keys[i])
        {
            free(shell->keys[i]);
            i++;
        }
        free(shell->keys);
        shell->keys = NULL;
    }
    if(tokens->tokens || tokens->type)
    {
        int i = 0;
        while(tokens->tokens[i])
        {
            free(tokens->tokens[i]);
            i++;
        }
        tokens->tokens = NULL;
    }
    if(exec->args)
    {
        free(exec->args);
        exec->args = NULL;
    }
    if (exec->in != -1)
    {
        close(exec->in);
        exec->in = -1;
    }
    if (exec->out != -1)
    {
        close(exec->out);
        exec->out = -1;
    }
}*/

void cleanup_shell(t_shell *shell)
{
    if (shell->cmds)
    {
        ft_lstclear(&(shell->cmds), ft_free); // Ensure ft_free handles the exec node cleanup.
        shell->cmds = NULL;
    }
    if (shell->keys)
    {
        int i = 0;
        while (shell->keys[i])
        {
            free(shell->keys[i]);
            i++;
        }
        free(shell->keys);
        shell->keys = NULL;
    }
}


int main(int ac, char **av, char **envp)
{
    t_shell shell;
   // t_exec  exec;
   // t_tokens tokens = {0,0,0,NULL};
    char    *line;
    t_list  *command_list;

	shell.cmds = NULL;
	shell.mpid = getpid();
    line = NULL;
	command_list = NULL;
	env_init(envp, &shell);  
    rl_bind_key('\t', handle_tab);
    while (ac && av)
    {
        signal(SIGINT, sigint_handler);
        signal(SIGQUIT,SIG_IGN);
		line = readline("minishell🔥$ ");
        if (!line)
        {
            printf("exit\n");
            break ;
        }
        if (*line)
        {
            add_history(line);
            tokenize_commands(&line, &command_list, &shell);
        }  
        shell.cmds = command_list;
		if (command_list)
			cmd_execute(&shell, command_list);
		ft_lstclear(&command_list, ft_free);
		command_list = NULL;
        shell.cmds = NULL;
        free(line);
      // printf("%d\n", g_env.exit_status);
    }
    cleanup_shell(&shell);
    return (g_env.exit_status);
}
