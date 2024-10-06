/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_token_set.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 11:47:09 by yfontene          #+#    #+#             */
/*   Updated: 2024/10/06 23:02:53 by yfontene         ###   ########.fr       */
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

void filler_stokens(char **cmds, t_tokens **token, int nbr, t_shell *shell)
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
    }
    (*token)[i].tokens[j] = NULL;
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
            char *cmd = ft_substr(line, start, i - start);
            cmd = remove_quotes(cmd);
            ft_lstadd_back(&cmd_list, ft_lstnew(cmd));
            start = i + 1;
        }
        i++;
    }
    if (start < i)
    {
        char *cmd = ft_substr(line, start, i - start);
        cmd = remove_quotes(cmd);
        ft_lstadd_back(&cmd_list, ft_lstnew(cmd));
    }
    cmds = lst_to_array(cmd_list);
    ft_lstclear(&cmd_list, free);
    if (!cmds)
        ft_error("Failed to split commands by pipe\n", 1);
    return cmds;
}

void tokenize_commands(char **cmdstr, t_list **commands_list, t_shell *shell)
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
}

