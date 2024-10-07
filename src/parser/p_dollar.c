/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_dollar.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eliskam <eliskam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 15:21:10 by yfontene          #+#    #+#             */
/*   Updated: 2024/10/07 16:31:23 by eliskam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include "../exec/execute.h"

void    dollar_replace(char **token, int i, t_shell *shell)
{
    char *str = ft_strdup(*token);
    free(*token);
    *token = NULL;
    if (str[i] == '$' && str[i + 1] == '?')
        *token = ft_itoa(g_exit_status);
    else
    {
        char **sorted_env = dup_array(shell->keys);
        sort_array(sorted_env);
        *token = find_env_value(str, i, sorted_env);
        free_str_array(sorted_env);
    }
    if (*token == NULL)
    {
        ft_error("Erro ao expandir variável", 1);
    }
    free(str);
}


char    **dollar_spaces_split(char **old, int i)
{
    int j;
    int k;
    char **new;
    char **content;

    j = 0;
    k = 0;
    content = ft_split(old[i], ' ');
    new = malloc(sizeof(char *) * (count(old) + count(content)));
    while (j++ < i)
        new[j] = ft_strdup(old[j]);
    while (content[k++])
    {
        new[j] = ft_strdup(content[k]);
        j++;
    }
    k = i;
    while (old[++k])
    {
        new[j] = ft_strdup(old[k]);
        j++;
    }
    new[j] = NULL;
    free_dollar(old, content);
    return (new);
}

void arg_type(t_tokens *token, int oldsize, int newsize, int i)
{
    int j;
    int k;
    int *new_type;

    j = 0;
    k = 0;
	new_type = malloc(sizeof(int) * newsize);
	while (j++ < i)
		new_type[j] = token->type[j];
	while (k++ < newsize - oldsize + 1)
	{
		if (k == 0)
			new_type[j] = PROTECTED_DOLLAR;
		else
			new_type[j] = PROTECTED_QUOTE;
		j++;
	}
	k = i;
	while (++k < oldsize)
	{
		new_type[j] = token->type[k];
		j++;
	}
	free(token->type);
	token->type = new_type;
}

int	dollar_aux_config(t_tokens *token, int *i, t_data *data)
{
    if (token->tokens[*i][0] == '$' && token->tokens[*i][1] == '?')
        dollar_replace(&(token->tokens[*i]), *i, data->shell);
	else if ((token->tokens[*i][1] == '$' && *i) || (*i == 0 && token->tokens[*i][0] == '$'))
	{
		dollar_replace(&(token->tokens[*i]), *i, data->shell);
		token->tokens = dollar_spaces_split(token->tokens, *i);
		data->presence = 1;
		data->newlen = count(token->tokens);
		if (data->oldlen != data->newlen)
			arg_type(token, data->oldlen, data->newlen, *i);
		else if (token->type[*i] < 0)
			token->type[*i] = -PROTECTED_DOLLAR;
		else
			token->type[*i] = PROTECTED_DOLLAR;
		if (token->type[0] == PROTECTED_DOLLAR || token->type[0] == -PROTECTED_DOLLAR)
			token->type[*i] = -CMD;
		(*i) += data->newlen - data->oldlen;
		data->oldlen = data->newlen;
		return (1);
	}
	return (0);
}

char *dollar_config(char *str, int pos, t_shell *shell)
{
    char *expanded_value;
    char **sorted_env;

    if (str[pos] == '$')
    {
        if (str[pos + 1] == '?')
            return ft_itoa(g_exit_status);
        else if (ft_isalnum(str[pos + 1]) || str[pos + 1] == '_' || ft_isalpha(str[pos + 1]))
        {
            sorted_env = dup_array(shell->keys);
            sort_array(sorted_env);
            expanded_value = find_env_value(str, pos, sorted_env);
            free_str_array(sorted_env);
            return expanded_value;
        }
        else
            return ft_strdup("$");
    }
    return ft_strdup(str);
}

