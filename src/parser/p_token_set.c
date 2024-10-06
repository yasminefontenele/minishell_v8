/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_token_set.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 11:47:09 by yfontene          #+#    #+#             */
/*   Updated: 2024/10/06 12:39:51 by yfontene         ###   ########.fr       */
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

/*void filler_stokens(char **cmds, t_tokens **token, int nbr)
{
    int i;

    i = 0;
    (*token)[i].nbr = count_token(cmds[i]);
    (*token)[i].pipe = nbr;
    (*token)[i].tokens = token_split(cmds[i], (*token)[i].nbr);
    (*token)[i].type = set_token((*token)[i]);
    (*token)[i] = dollar_config((*token) +  i);

    (*token)[i] = dollar_config((*token) + i);

    
}*/
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

//before m_export
/*void filler_stokens(char **cmds, t_tokens **token, int nbr, t_shell *shell)
{
    int i = 0;
    int j;
    
    printf("o comando entrou em filler_stokens\n");
    (*token)[i].nbr = count_token(cmds[i]);
    (*token)[i].pipe = nbr;
    (*token)[i].tokens = token_split(cmds[i], (*token)[i].nbr);

   
    if (!(*token)[i].tokens)
    {
        ft_error("Token split failed\n", 1);
        return;
    }

    (*token)[i].type = set_token((*token)[i]);

    if (check_unclosed_quotes(cmds[i]))
    {
        ft_error("Syntax error: unclosed quotes\n", 0);
        return;
    }

    j = 0;
    while ((*token)[i].tokens[j])// Enquanto houver tokens
    {
        printf("Token %d: %s\n", j, (*token)[i].tokens[j]);

        // Processar variáveis com dólar
        if ((*token)[i].tokens[j][0] == '$') {
            (*token)[i].tokens[j] = dollar_config((*token)[i].tokens[j], 0, shell);
            printf("Token after dollar config: %s\n", (*token)[i].tokens[j]);
        }

        // Verificar e processar as aspas
        if ((*token)[i].tokens[j][0] == '\'' || (*token)[i].tokens[j][0] == '\"')// Se o token começa com aspas
        {
            printf("Processing quotes for token %s\n", (*token)[i].tokens[j]);
            (*token)[i].tokens[j] = process_quotes((*token)[i].tokens[j], shell);// Processar aspas
            printf("Token after quote processing: %s\n", (*token)[i].tokens[j]);
        }
        j++;
    }

    (*token)[i].tokens[j] = NULL;
}*/

/*while ((*token)[i].tokens[j])
{
    // Verifica se o token está entre aspas duplas
    if ((*token)[i].tokens[j][0] == '\"' && (*token)[i].tokens[j][ft_strlen((*token)[i].tokens[j]) - 1] == '\"')
    {
        // Remove as aspas duplas
        (*token)[i].tokens[j] = remove_quotes((*token)[i].tokens[j]);
        
        // Agora percorre a string para encontrar variáveis de ambiente
        char *expanded_str = ft_strdup(""); // String temporária para construir a nova
        char *temp_str = (*token)[i].tokens[j];
        int k = 0;
        
        while (temp_str[k])
        {
            if (temp_str[k] == '$')
            {
                // Expande a variável de ambiente
                char *expanded_var = dollar_config(temp_str, k, shell);
                expanded_str = ft_strjoin(expanded_str, expanded_var); // Junta a string expandida
                free(expanded_var);
                // Pula para o próximo caractere após a variável
                while (temp_str[k] && (ft_isalnum(temp_str[k]) || temp_str[k] == '_' || temp_str[k] == '?'))
                    k++;
            }
            else
            {
                // Copia o caractere normal
                char c[2] = { temp_str[k], '\0' };
                expanded_str = ft_strjoin(expanded_str, c);
                k++;
            }
        }

        // Substitui o token pelo valor expandido
        free((*token)[i].tokens[j]);
        (*token)[i].tokens[j] = expanded_str;
    }
    else if ((*token)[i].tokens[j][0] == '\'' && (*token)[i].tokens[j][ft_strlen((*token)[i].tokens[j]) - 1] == '\'')
    {
        // Remove aspas simples (sem expansão)
        (*token)[i].tokens[j] = remove_quotes((*token)[i].tokens[j]);
    }
    else if ((*token)[i].tokens[j][0] == '$')
    {
        // Expansão de variável se começar com $
        (*token)[i].tokens[j] = dollar_config((*token)[i].tokens[j], 0, shell);
    }
    else
    {
        // Caso contrário, mantêm o token original
        (*token)[i].tokens[j] = ft_strdup((*token)[i].tokens[j]);
    }

    j++;
}*/


void filler_stokens(char **cmds, t_tokens **token, int nbr, t_shell *shell)
{
    //printf("o codigo entrou em filler_stokens\n");
    int i;
    int j;
    
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
        /*printf("Token antes: %s\n", (*token)[i].tokens[j]);
        if ((*token)[i].tokens[j][0] == '\"' && (*token)[i].tokens[j][ft_strlen((*token)[i].tokens[j]) - 1] == '\"')
        {
            (*token)[i].tokens[j] = remove_quotes((*token)[i].tokens[j]);
            //printf("Token antes de dollar_config: %s\n", (*token)[i].tokens[j]);
            (*token)[i].tokens[j] = dollar_config((*token)[i].tokens[j], 0, shell);
        }*/
        if ((*token)[i].tokens[j][0] == '\"' && (*token)[i].tokens[j][ft_strlen((*token)[i].tokens[j]) - 1] == '\"')
        {
            char *original_token = remove_quotes((*token)[i].tokens[j]);
            char *expanded_str = ft_strdup(""); // String temporária para construir a nova
            int k = 0;
            
            while (original_token[k])
            {
                if (original_token[k] == '$')
                {
                    // Expande a variável de ambiente
                    char *expanded_var = dollar_config(original_token, k, shell);

                    // Avança a posição até o fim do nome da variável
                    int var_len = 1; // Conta o '$'
                    if (ft_isalpha(original_token[k + 1]) || original_token[k + 1] == '_')
                    {
                        var_len++;
                        while (ft_isalnum(original_token[k + var_len]))
                            var_len++;
                    }

                    // Concatena a parte expandida à string resultante
                    expanded_str = ft_strjoin(expanded_str, expanded_var);
                    free(expanded_var);

                    // Pula para o próximo caractere após a variável
                    k += var_len;
                }
                else
                {
                    // Concatena caracteres que não são parte de uma variável
                   char c[2] = { original_token[k], '\0' };
                    expanded_str = ft_strjoin(expanded_str, c);
                    k++;
                }
            }
            free((*token)[i].tokens[j]);
            free(original_token); // Libera a string sem aspas
            (*token)[i].tokens[j] = expanded_str;
        }
        else if ((*token)[i].tokens[j][0] == '\'' && (*token)[i].tokens[j][ft_strlen((*token)[i].tokens[j]) - 1] == '\'')
        {
            (*token)[i].tokens[j] = remove_quotes((*token)[i].tokens[j]);
            // Não chama dollar_config aqui, pois está entre aspas simples
        }
        else if((*token)[i].tokens[j][0] == '$')
        {
            if (ft_isalpha((*token)[i].tokens[j][1]) || (*token)[i].tokens[j][1] == '_' || (*token)[i].tokens[j][1] == '?')
            {
                (*token)[i].tokens[j] = dollar_config((*token)[i].tokens[j], 0, shell);
            }
            else
            {
                // Manter o token original se não for uma variável válida
                (*token)[i].tokens[j] = ft_strdup((*token)[i].tokens[j]);
            }
        }
    
    j++;
    }

    if (j > 1 && ft_strchr((*token)[i].tokens[j - 2], '='))//
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
    int size = ft_lstsize(list);
    char **array = malloc(sizeof(char *) * (size + 1));
    int i = 0;

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
    //printf("o codigo entrou em split_command_line\n");
    char **cmds;//array de strings
    int i = 0;
    int start = 0;//indice de inicio da string
    bool in_quotes = false;//flag para saber se está dentro de aspas
    char quote_type = '\0';
    t_list *cmd_list = NULL;
    //printf("line: %s\n", line);
    while (line[i])
    {
        // Verificar se está dentro de aspas
        if ((line[i] == '"' || line[i] == '\'') && (quote_type == '\0' || quote_type == line[i]))
        {   //printf("Aspas detectada em line[%d]: %c\n", i, line[i]);
            if (!in_quotes)// Se não estiver dentro de aspas
            {
                quote_type = line[i];
                in_quotes = true;
            }
            else if (line[i] == quote_type)// Se estiver dentro de aspas
            {
                in_quotes = false;
                quote_type = '\0';
                //printf("Aspas fechadas: %c em posição %d\n", line[i], i);
            }
        }
        if (line[i] == '|' && !in_quotes)// Se encontrar um pipe e não estiver dentro de aspas
        {
            char *cmd = ft_substr(line, start, i - start);// Extrair o comando
            cmd = remove_quotes(cmd);
            //printf("Comando extraído: '%s'\n", cmd);
            ft_lstadd_back(&cmd_list, ft_lstnew(cmd));// Adicionar o comando à lista
            start = i + 1;// Atualizar o índice de início
        }
        i++;
    }
    if (start < i)// Se houver um comando após o último pipe
    {
        char *cmd = ft_substr(line, start, i - start);// Extrair o comando
        cmd = remove_quotes(cmd);
        //printf("Comando final extraído: '%s'\n", cmd);
        ft_lstadd_back(&cmd_list, ft_lstnew(cmd));// Adicionar o comando à lista
    }
    cmds = lst_to_array(cmd_list);// Converter a lista para um array
    ft_lstclear(&cmd_list, free);// Limpar a lista
    if (!cmds)
        ft_error("Failed to split commands by pipe\n", 1);
        
    //printf("Linha após split_command_line: %s\n", line);

    return cmds;
}

void tokenize_commands(char **cmdstr, t_list **commands_list, t_shell *shell)
{
    //printf("o codigo entrou em tokenize_commands\n");
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
            //printf("Token %d: %s\n", j, token->tokens[j]);
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




