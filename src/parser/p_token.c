/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_token.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 18:11:10 by yfontene          #+#    #+#             */
/*   Updated: 2024/10/07 20:21:23 by yfontene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int token_dollar_end(char *str, int i)
{
    if (str[i + 1] && str[i + 1] == '$')
        return (1 + 2);
    i++;
    while (str[i])
    {
        if (str[i] == '$' || str[i] == ' ' || str[i] == '\t'
			|| str[i] == '\'' || str[i] == '\"' || str[i] == '>'
			|| str[i] == '<' || str[i] == '\0')//
            return (i);
        else
            i++;
    }
    return (i);
}

int token_word_end(char *str, int i)
{
    while (str[i])
    {
        if (str[i] == '$' || str[i] == ' ' || str[i] == '\t'
            || str[i] == '>' || str[i] == '<' || str[i] == '\0')
            return (i);
        else if ((str[i] == '\'' || str[i] == '\"')
            && (count_backslash(str, i) % 2 == 0))
            return (i);
        else
            i++;
    }
    return (i);
}

int token_quotes_end(char *str, int i)
{
    int j = i + 1;
    char quote_char = str[i];

    while (str[j])
    {
        if (str[j] == quote_char)
            return j;
        j++;
    }
    ft_error("Error: unmatched quote\n", 1);
    return -1;
}

int token_redir_end(char *str, int i)
{
    int j;

    j = i;
    while (str[i])
    {
        if (str[j] == str[i])
            i++;
        else
            return (i);
    }
    return (i);
}

char *parse_next_token(char *line, int reset, int keep_quotes)
{
    static int current_pos;
    int start = 0;
    int end = 0;
    char quote_char;

    if (reset == 0)
        current_pos = 0;
    current_pos = skip_space(line, current_pos);
    if (!line[current_pos])
        return NULL;
    if (line[current_pos] == '\"' || line[current_pos] == '\'')
    {
        quote_char = line[current_pos++];
        start = current_pos;
        while (line[current_pos] && line[current_pos] != quote_char)
            current_pos++;
        if (line[current_pos] == quote_char)
        {
            end = current_pos;
            current_pos++;
        }
        else
        {
            ft_error("Error: unmatched quote\n", 1);
            return NULL;
        }
        if (keep_quotes)
        {
            start--;
            end++;
        }
        return (extract_substring(line, start, end));
    }
    else if (line[current_pos] == '>' || line[current_pos] == '<')
    {
        start = current_pos++;
        if (line[current_pos] == line[start])
            current_pos++;
        end = current_pos;
        return (extract_substring(line, start, end));
    }
    else
    {
        start = current_pos;
        while (line[current_pos] && line[current_pos] != ' ' && line[current_pos] != '\t' &&
               line[current_pos] != '\"' && line[current_pos] != '\'' &&
                line[current_pos] != '>' && line[current_pos] != '<')
        {
            current_pos++;
        }
        end = current_pos;
        return (extract_substring(line, start, end));
    }
    return (NULL);
}

