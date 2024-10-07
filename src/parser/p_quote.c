/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_quote.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 08:59:10 by yfontene          #+#    #+#             */
/*   Updated: 2024/10/07 14:08:23 by yfontene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int dollar_presence(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == '$')
            return (1);
        i++;
    }
    return (0);
}

char *quotes_expand(char *content, int i, t_shell *shell)
{
    int nbr_of_dollars;
    char *tmp;

    if (content[i] == '\"')
    {
        printf("Double quotes found in: %s\n", content);
        tmp = ft_substr(content, i + 1, ft_strlen(content) - i - 2);
        printf("Content between double quotes: %s\n", tmp);
        nbr_of_dollars = dollar_presence(tmp);
        if (nbr_of_dollars != 0)
        {
            tmp = dollar_config(tmp, 0, shell);
            printf("After dollar expansion: %s\n", tmp);
        }
        return tmp;
    }
    if (content[i] == '\'')
        return ft_substr(content, i + 1, ft_strlen(content) - i - 2);
    return ft_strdup(content);
}

char *process_quotes(char *str, t_shell *shell)
{
    int i = 0;
    if (str[i] == '\'')
    {
        i++;
        while (str[i] && str[i] != '\'')
            i++;
        return ft_substr(str, 1, i - 1);
    }
    if (str[i] == '\"')
    {
        return quotes_expand(str, i, shell);
    }
    return ft_strdup(str);
}

t_tokens process_quotes_tokens(t_tokens tokens, t_shell *shell)
{
    int i;
    int j;
    char *tmp_token;

    i = 0;
    printf("o codigo entrou em process_quotes_tokens\n");
    while (tokens.tokens[i])
    {
        j = 0;
        // Verifica se o token tem aspas no começo
        if (tokens.tokens[i][j] == '\'' || tokens.tokens[i][j] == '\"')
        {
            tmp_token = tokens.tokens[i];// Salva o token original
            tokens.tokens[i] = quotes_expand(tokens.tokens[i], j, shell);// Processa as aspas
            free(tmp_token);
        }
        i++;
    }
    return (tokens);
}

void exec_process_quotes(t_tokens *tokens, t_shell *shell)
{
    printf("entrou em exex_process_qutes\n");
    int i;

    i = 0;
    while (i < tokens[0].pipe)
    {
        tokens[i] = process_quotes_tokens(tokens[i], shell);
        int j = 0;
        while (tokens[i].tokens[j])
        {
            printf("Token após processamento: %s\n", tokens[i].tokens[j]);
            j++;
        }
        i++;
    }
}

bool valid_quotes(const char *str)
{
    char quote_type = '\0';
    bool in_quotes = false;
    int i = 0;

    while (str[i])
    {
        if ((str[i] == '"' || str[i] == '\'') && (quote_type == '\0' || quote_type == str[i]))
        {
            if (!in_quotes)
            {
                quote_type = str[i];
                in_quotes = true;
            }
            else if (str[i] == quote_type)
            {
                in_quotes = false;
                quote_type = '\0';
            }
        }
        i++;
    }
    if (in_quotes)
        return false;
    
    return true;
}

