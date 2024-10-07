/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_token_set.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 11:47:09 by yfontene          #+#    #+#             */
/*   Updated: 2024/10/07 23:28:02 by yfontene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include "../exec/execute.h"


void		type_of_separator(int *type_of, char **token)
{
    int i;

    i = 0;
    while (token[i])
    {
        if (token[i][0] != ' ' && token[i][0] != '\t')
            type_of[i] = -1;
        else
            type_of[i] = 1;
    i++;
    }
}

int check_unclosed_quotes(char *str)
{
    int single_quote = 0;
    int double_quote = 0;
    int i = 0;

    while (str[i]) {
        if (str[i] == '\'')
            single_quote = !single_quote;
        else if (str[i] == '\"')
            double_quote = !double_quote;
        i++;
    }
    return single_quote || double_quote;
}
//LAST VALID VERSION BEFORE SPLIT
/*void filler_stokens(char **cmds, t_tokens **token, int nbr, t_shell *shell)
{
    int i;
    int j;
    int k;
    char *original_token;
    char *expanded_str;
    char *expanded_var;
    
    i = 0;
    if (!valid_quotes(cmds[i]))
    {
        ft_error("Unclosed or unmatched quotes found", 1);
        return;
    }
    (*token)[i].nbr = count_token(cmds[i]);
    (*token)[i].pipe = nbr;
    (*token)[i].tokens = token_split(cmds[i], (*token)[i].nbr);
    if (!(*token)[i].tokens)
    {
        ft_error("Token split failed\n", 1);
        return ;
    }
    (*token)[i].type = set_token((*token)[i]);
    j = 0;
    while ((*token)[i].tokens[j])
    {
        if ((*token)[i].tokens[j][0] == '\"' && (*token)[i].tokens[j][ft_strlen((*token)[i].tokens[j]) - 1] == '\"')
        {
            original_token = remove_quotes((*token)[i].tokens[j]);
            expanded_str = ft_strdup("");
            k = 0;
            while (original_token[k])
            {
                if (original_token[k] == '$')
                {
                    expanded_var = dollar_config(original_token, k, shell);
                    int var_len = 1;
                    if (ft_isalpha(original_token[k + 1]) || original_token[k + 1] == '_')
                    {
                        var_len++;
                        while (ft_isalnum(original_token[k + var_len]))
                            var_len++;
                    }
                    expanded_str = ft_strjoin(expanded_str, expanded_var);
                    free(expanded_var);
                    k += var_len;
                }
                else
                {
                    char c[2] = { original_token[k], '\0' };
                    expanded_str = ft_strjoin(expanded_str, c);
                    k++;
                }
            }
            free((*token)[i].tokens[j]);
            free(original_token);
            (*token)[i].tokens[j] = expanded_str;
        }
        else if ((*token)[i].tokens[j][0] == '\'' && (*token)[i].tokens[j][ft_strlen((*token)[i].tokens[j]) - 1] == '\'')
            (*token)[i].tokens[j] = remove_quotes((*token)[i].tokens[j]);
        else if((*token)[i].tokens[j][0] == '$')
        {
            if (ft_isalpha((*token)[i].tokens[j][1]) || (*token)[i].tokens[j][1] == '_' || (*token)[i].tokens[j][1] == '?')
                (*token)[i].tokens[j] = dollar_config((*token)[i].tokens[j], 0, shell);
            else
                (*token)[i].tokens[j] = ft_strdup((*token)[i].tokens[j]);
        }
        else
            (*token)[i].tokens[j] = ft_strdup((*token)[i].tokens[j]);
                
        j++;
    }
    if (j > 1 && ft_strchr((*token)[i].tokens[j - 2], '='))
    {
        char *temp = ft_strjoin((*token)[i].tokens[j - 2], (*token)[i].tokens[j - 1]);
        free((*token)[i].tokens[j - 2]);
        free((*token)[i].tokens[j - 1]);
        (*token)[i].tokens[j - 2] = temp;
        (*token)[i].tokens[j - 1] = NULL;
        free(temp);
    }
    (*token)[i].tokens[j] = NULL;
}*/

void initialize_token(char **cmds, t_tokens **token, int i, int nbr)
{
    if (!valid_quotes(cmds[i]))
    {
        ft_error("Unclosed or unmatched quotes found", 1);
        return;
    }
    (*token)[i].nbr = count_token(cmds[i]);
    (*token)[i].pipe = nbr;
    (*token)[i].tokens = token_split(cmds[i], (*token)[i].nbr);
    if (!(*token)[i].tokens)
    {
        ft_error("Token split failed\n", 1);
        return ;
    }
    (*token)[i].type = set_token((*token)[i]);
}

char *expand_variable(char *original_token, int *k, t_shell *shell)
{
    char *expanded_var = dollar_config(original_token, *k, shell);
    int var_len = 1;
    
    if (ft_isalpha(original_token[*k + 1]) || original_token[*k + 1] == '_')
    {
        var_len++;
        while (ft_isalnum(original_token[*k + var_len]))
            var_len++;
    }
    *k += var_len;
    return expanded_var;
}

char *process_expansion(char *original_token, t_shell *shell)
{
    char *expanded_str = ft_strdup("");
    int k = 0;

    while (original_token[k])
    {
        if (original_token[k] == '$')
        {
            char *expanded_var = expand_variable(original_token, &k, shell);
            expanded_str = ft_strjoin(expanded_str, expanded_var);
            free(expanded_var);
        }
        else
        {
            char c[2] = { original_token[k], '\0' };
            expanded_str = ft_strjoin(expanded_str, c);
            k++;
        }
    }
    return expanded_str;
}

void process_token(t_tokens *token, int i, int j, t_shell *shell)
{
    char *original_token;
    char *expanded_str;
    
    if (token[i].tokens[j][0] == '\"' && token[i].tokens[j][ft_strlen(token[i].tokens[j]) - 1] == '\"')
    {
        original_token = remove_quotes(token[i].tokens[j]);
        expanded_str = process_expansion(original_token, shell);
        free(token[i].tokens[j]);
        free(original_token);
        token[i].tokens[j] = expanded_str;
    }
    else if (token[i].tokens[j][0] == '\'' && token[i].tokens[j][ft_strlen(token[i].tokens[j]) - 1] == '\'')
        token[i].tokens[j] = remove_quotes(token[i].tokens[j]);
    else if (token[i].tokens[j][0] == '$')
    {
        if (ft_isalpha(token[i].tokens[j][1]) || token[i].tokens[j][1] == '_' || token[i].tokens[j][1] == '?')
            token[i].tokens[j] = dollar_config(token[i].tokens[j], 0, shell);
        else
            token[i].tokens[j] = ft_strdup(token[i].tokens[j]);
    }
    else
        token[i].tokens[j] = ft_strdup(token[i].tokens[j]);
}

void combine_tokens(t_tokens *token, int i, int j)
{
    if (j > 1 && ft_strchr(token[i].tokens[j - 2], '='))
    {
        char *temp = ft_strjoin(token[i].tokens[j - 2], token[i].tokens[j - 1]);
        free(token[i].tokens[j - 2]);
        free(token[i].tokens[j - 1]);
        token[i].tokens[j - 2] = temp;
        token[i].tokens[j - 1] = NULL;
    }
    token[i].tokens[j] = NULL;
}

void filler_stokens(char **cmds, t_tokens **token, int nbr, t_shell *shell)
{
    int i = 0;
    int j = 0;

    initialize_token(cmds, token, i, nbr);
    
    while ((*token)[i].tokens[j])
    {
        process_token(*token, i, j, shell);
        j++;
    }

    combine_tokens(*token, i, j);
}

char **lst_to_array(t_list *list)
{
    int size;
    char **array;
    int i;

    size = ft_lstsize(list);
    array = malloc(sizeof(char *) * (size + 1));
    i = 0;
    if (!array)
        return NULL;
    while (list)
    {
        array[i] = ft_strdup((char *)list->content);
        list = list->next;
        i++;
    }
    array[i] = NULL;
    return array;
}

char **split_command_line(char *line)
{
    char **cmds;
    int i = 0;
    int start = 0;
    bool in_quotes = false;
    char quote_type = '\0';
    t_list *cmd_list = NULL;
    char *cmd;

    while (line[i])
    {

        if ((line[i] == '"' || line[i] == '\'') && (quote_type == '\0' || quote_type == line[i]))
        {
            if (!in_quotes)
            {
                quote_type = line[i];
                in_quotes = true;
            }
            else if (line[i] == quote_type)
            {
                in_quotes = false;
                quote_type = '\0';
            }
        }
        if (line[i] == '|' && !in_quotes)
        {
            cmd = ft_substr(line, start, i - start);
            char *clean_cmd = remove_quotes(cmd);
            free(cmd);
            cmd = clean_cmd;
            ft_lstadd_back(&cmd_list, ft_lstnew(cmd));
            start = i + 1;
        }
        i++;
    }
    if (start < i)
    {
        cmd = ft_substr(line, start, i - start);
        char *clean_cmd = remove_quotes(cmd);
        free(cmd);
        cmd = clean_cmd;
        ft_lstadd_back(&cmd_list, ft_lstnew(cmd));
    }
    cmds = lst_to_array(cmd_list);
    ft_lstclear(&cmd_list, free);
    if (!cmds)
        ft_error("Failed to split commands by pipe\n", 1);
    return cmds;
}

//LAST VALID VERSION BEFORE SPLIT
/*void tokenize_commands(char **cmdstr, t_list **commands_list, t_shell *shell)
{
    int i;
    int j;
    char **cmds;
    t_tokens *token;
    t_exec *exec_node;
    t_list *new_node;

    cmds = split_command_line(*cmdstr);
    if (!cmds)
    {
        ft_error("Error: split_command_line returned NULL\n", 1);
        return;
    }
    i = 0;
    while (cmds[i])
    {
        token = malloc(sizeof(t_tokens));
        if (!token)
            ft_error("Malloc failed in tokenize_commands\n", 1); 
        filler_stokens(&cmds[i], &token, i, shell);//
        exec_node = malloc(sizeof(t_exec));
        if (!exec_node)
            ft_error("Malloc failed for exec_node", 1);
        exec_node->args = malloc(sizeof(char *) * (count_token(cmds[i]) + 1));
        if (!exec_node->args)
            ft_error("Malloc failed for exec_node->args", 1);
        j = 0;
        while (token->tokens[j])
        {
            exec_node->args[j] = ft_strdup(token->tokens[j]);
            j++;
        }
        exec_node->args[j] = NULL;
        new_node = ft_lstnew(exec_node);
        exec_node->path = NULL;
        exec_node->in = 0;
        exec_node->out = 1;
        if (!(exec_node)->args[0] || ft_strlen((exec_node)->args[0]) == 0 || ft_str_is_space((exec_node)->args[0]))
            return ; 
        if (!new_node)
            ft_error("Malloc failed for new_node", 1);
        ft_lstadd_back(commands_list, new_node);
        free_tokens(token);
        free(token->type);
        free(token);
        i++;
    }
    free_split(cmds);
}*/

char **split_command_and_check(char *cmdstr)
{
    char **cmds = split_command_line(cmdstr);
    if (!cmds)
    {
        ft_error("Error: split_command_line returned NULL\n", 1);
        return NULL;
    }
    return cmds;
}

t_exec *init_exec_node(char *cmd)
{
    t_exec *exec_node = malloc(sizeof(t_exec));
    if (!exec_node)
    {
        ft_error("Malloc failed for exec_node", 1);
        return NULL;
    }
    exec_node->args = malloc(sizeof(char *) * (count_token(cmd) + 1));
    if (!exec_node->args)
    {
        ft_error("Malloc failed for exec_node->args", 1);
        free(exec_node);
        return NULL;
    }
    exec_node->path = NULL;
    exec_node->in = 0;
    exec_node->out = 1;
    return exec_node;
}

void fill_exec_node_args(t_exec *exec_node, t_tokens *token)
{
    int j = 0;
    while (token->tokens[j])
        j++;

    exec_node->args = malloc(sizeof(char *) * (j + 1));
    if (!exec_node->args)
    {
        ft_error("Malloc failed for exec_node->args", 1);
        free_exec_node(exec_node);
        return ;
    }
    j = 0;
    while (token->tokens[j])
    {
        exec_node->args[j] = ft_strdup(token->tokens[j]);
        j++;
    }
    exec_node->args[j] = NULL;
}

void add_to_command_list(t_exec *exec_node, t_list **commands_list)
{
    t_list *new_node = ft_lstnew(exec_node);
    if (!new_node)
    {
        ft_error("Malloc failed for new_node", 1);
        return;
    }
    ft_lstadd_back(commands_list, new_node);
}

t_tokens *create_and_process_tokens(char *cmd, int index, t_shell *shell)
{
    t_tokens *token = malloc(sizeof(t_tokens));
    if (!token)
    {
        ft_error("Malloc failed in create_and_process_tokens\n", 1);
        return NULL;
    }
    filler_stokens(&cmd, &token, index, shell);
    return token;
}

t_exec *create_and_fill_exec_node(char *cmd, t_tokens *token)
{
    t_exec *exec_node = init_exec_node(cmd);
    if (!exec_node)
        return NULL;

    fill_exec_node_args(exec_node, token);

    if (!(exec_node->args[0]) || ft_strlen(exec_node->args[0]) == 0 || ft_str_is_space(exec_node->args[0]))
        return NULL;

    return exec_node;
}

void process_single_command(char *cmd, t_list **commands_list, t_shell *shell, int index)
{
    t_tokens *token = create_and_process_tokens(cmd, index, shell);
    if (!token)
        return;

    t_exec *exec_node = create_and_fill_exec_node(cmd, token);
    if (!exec_node)
    {
        free_tokens(token);
        free_exec_node(exec_node);
        return ;
    }

    add_to_command_list(exec_node, commands_list);

    free_tokens(token);
    free(token->type);
    free(token);
}

void tokenize_commands(char **cmdstr, t_list **commands_list, t_shell *shell)
{
    char **cmds = split_command_and_check(*cmdstr);
    if (!cmds)
        return;

    int i = 0;
    while (cmds[i])
    {
        process_single_command(cmds[i], commands_list, shell, i);
        i++;
    }

    free_split(cmds);
}


