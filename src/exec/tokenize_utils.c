////ADD HEADER FILE

#include "execute.h"
#include "../../minishell.h"


void handle_memory_failure(char *message)
{
    ft_error(message, 1);
}

t_tokens *create_and_fill_tokens(char **cmd, int index, t_shell *shell)
{
    t_tokens *token;

    token = malloc(sizeof(t_tokens));
    if (!token)
        handle_memory_failure("Malloc failed in tokenize_commands\n");
    filler_stokens(cmd, &token, index, shell);
    return (token);
}

void remove_quotes_from_args(t_exec *node)
{
    int i = 0;
    char *temp;

    while (node->args[i])
    {
        temp = remove_quotes(node->args[i]);
        if (temp)
        {
            free(node->args[i]);
            node->args[i] = temp;
        }
        i++;
    }
}