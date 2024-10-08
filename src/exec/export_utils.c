//HEADER FILE

#include "execute.h"


int m_expr(char **args)
{
    int num1, num2;
    char op;

    if (!args[1] || !args[2] || !args[3])
        return 1;
    num1 = atoi(args[1]);
    op = args[2][0];
    num2 = atoi(args[3]);
    if (op == '+')
        printf("%d\n", num1 + num2);
    else if (op == '-')
        printf("%d\n", num1 - num2);
    else if (op == '*')
        printf("%d\n", num1 * num2);
    else if (op == '/')
    {
        if (num2 == 0)
        {
            printf("expr: divisão por zero\n");
            return (1);
        }
        printf("%d\n", num1 / num2);
    }
    else
    {
        printf("expr: operador inválido\n");
        return (1);
    }
    return (0);
}

void print_export(char **keys)
{
    int i;
    int j;
    char *temp;

    i = 0;
    while (keys[i] != NULL)
    {
        j = i + 1;
        while (keys[j] != NULL)
        {
            if (strcmp(keys[i], keys[j]) > 0)
            {
                temp = keys[i];
                keys[i] = keys[j];
                keys[j] = temp;
            }
            j++;
        }
        i++;
    }
    i = 0;
    while (keys[i])
    {
        printf("declare -x %s\n", keys[i]);
        i++;
    }
}

char *remove_quotes(char *token)
{
    int len = ft_strlen(token);
    char *new_token;
    int i;
    int j;
    
    i = 0;
    j = 0;
    new_token = malloc(len + 1);
    len = ft_strlen(token);
    if ((token[0] == '\'' || token[0] == '"') && (token[len - 1] == token[0])) {
        i++;
        len--; 
    }
    while (i < len) 
        new_token[j++] = token[i++];
    new_token[j] = '\0';
    return (new_token);
}

void remove_quotes_ifneeded(char **value)
{
    size_t len;

    len = ft_strlen(*value);
    if (len >= 2 && ((*value)[0] == '"' && (*value)[len - 1] == '"'))
    {
        (*value)[len - 1] = '\0';
        ft_memmove(*value, *value + 1, len - 1);
    }
    else if (len >= 2 && ((*value)[0] == '\'' && (*value)[len - 1] == '\''))
    {
        (*value)[len - 1] = '\0';
        ft_memmove(*value, *value + 1, len - 1);
    }
}

void split_var_value(char *arg, char **var, char **value)
{
    char *eq_pos;

    eq_pos = ft_strchr(arg, '=');
    if (!eq_pos)
    {
        *var = NULL;
        *value = NULL;
        return;
    }
    *var = ft_strndup(arg, eq_pos - arg);
    if (*(eq_pos + 1) != '\0')
    {
        *value = ft_strdup(eq_pos + 1);
        remove_quotes_ifneeded(value);
    }
    else
        *value = ft_strdup("");
}


/*

void split_var_value(char *arg, char **var, char **value)
{
    char *eq_pos;
    size_t len;

    eq_pos = ft_strchr(arg, '=');
    if (!eq_pos)
    {
        *var = NULL;
        *value = NULL;
        return;
    }
    *var = ft_strndup(arg, eq_pos - arg);
    if (*(eq_pos + 1) != '\0')
    {
        *value = ft_strdup(eq_pos + 1);
        len = ft_strlen(*value);
        if (len >= 2 && ((*value)[0] == '"' && (*value)[len - 1] == '"'))
        {
            (*value)[len - 1] = '\0';
            ft_memmove(*value, *value + 1, len - 1);
        }
        else if (len >= 2 && ((*value)[0] == '\'' && (*value)[len - 1] == '\''))
        {
            (*value)[len - 1] = '\0';
            ft_memmove(*value, *value + 1, len - 1);
        }
    }
    else
        *value = ft_strdup("");
}
*/