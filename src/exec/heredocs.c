//HEADER

#include "execute.h"

int read_input_lines(char *input_buffer[2], size_t limit_length, char *end_marker, char *error_message)
{
    int line_count;

    line_count = 0;
    while (g_exit_status != 130 && (input_buffer[0] == NULL ||
           ft_strncmp(input_buffer[0], end_marker, limit_length) != 0 || 
           ft_strlen(end_marker) != limit_length))
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
            return -1; // Indicate EOF
        }
        line_count++;
    }
    return (line_count); // Return the total line count
}

char *construct_here_document(char *input_buffer[2], size_t line_count)
{
    char *temp;

    while (line_count > 0)
    {
        temp = input_buffer[1];
        input_buffer[1] = ft_strjoin(temp, input_buffer[0]); // Join the new input
        free(temp);
        temp = ft_strjoin(input_buffer[1], "\n"); // Add newline
        free(input_buffer[1]);
        input_buffer[1] = temp;
        free(input_buffer[0]); // Free the current input
        input_buffer[0] = NULL;
        line_count--; // Decrement line count
    }

    return (input_buffer[1]); // Return the constructed here-document
}

char *read_here_document(char *input_buffer[2], size_t limit_length, char *end_marker, char *error_message)
{
    int line_count;

    line_count = read_input_lines(input_buffer, limit_length, end_marker, error_message);
    if (line_count == -1) // Check if EOF was encountered
        return NULL;

    if (line_count > 0)
        printf("Here-document contains %u lines.\n", line_count);

    return (construct_here_document(input_buffer, line_count)); // Build the final document
}

int create_here_document_fd(char *input_buffer[2], char *delimiter[2])
{
    int fd[2];

    g_exit_status = 0;
    if (pipe(fd) == -1)
    {
        m_perror(PIPE_READ, NULL, 1);
        return (-1);
    }
    input_buffer[1] = read_here_document(input_buffer, 0, delimiter[0], delimiter[1]);
    if (input_buffer[1] == NULL || ft_strlen(input_buffer[1]) == 0)
    {
        close(fd[PIPE_READ]);
        close(fd[PIPE_WRITE]);
        return (-1);
    }
    write(fd[PIPE_WRITE], input_buffer[1], ft_strlen(input_buffer[1]));
    free(input_buffer[1]);
    close(fd[PIPE_WRITE]);
    if (g_exit_status == 130)
    {
        close(fd[PIPE_READ]);
        return (-1);
    }
    return (fd[PIPE_READ]);
}

/*
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
*/