//HEADER

#include "execute.h"

char *read_here_document(char *input_buffer[2], size_t limit_length, char *end_marker, char *error_message)
{
    char *temp;
    size_t line_count;

    line_count = 0;
    while (g_exit_status != 130 && (input_buffer[0] == NULL
        || ft_strncmp(input_buffer[0], end_marker, limit_length) != 0 
        || ft_strlen(end_marker) != limit_length))
    {
        input_buffer[0] = readline("> ");
        if (input_buffer[0] == NULL)
        {
            printf("%s (wanted `%s`)\n", error_message, end_marker);
            break;
        }
         if (ft_strcmp(input_buffer[0], "EOF") == 0)
        {
            free(input_buffer[0]);
            return (NULL);
        }
        line_count++;
        temp = input_buffer[1];
        input_buffer[1] = ft_strjoin(input_buffer[1], input_buffer[0]);
        free(temp);
        temp = ft_strjoin(input_buffer[1], "\n");
        free(input_buffer[1]);
        input_buffer[1] = temp;
        free(input_buffer[0]);
        input_buffer[0] = NULL;
    }
    if (line_count > 0)
        printf("Here-document contains %zu lines.\n", line_count); 
    return (input_buffer[1]);
}
