#include "main.h"

ssize_t _getline(char **lineptr, size_t *n, FILE *stream) {
    if (*lineptr == NULL || *n == 0) {
        *n = 128;  // Initial buffer size
        *lineptr = (char *)malloc(*n);
        if (*lineptr == NULL) {
            return -1;  // Error allocating memory
        }
    }

    size_t pos = 0;
    int c;

    while ((c = fgetc(stream)) != EOF) {
         
        (*lineptr)[pos++] = (char)c;

        if (pos >= *n - 1) {
            *n *= 2;  // Double the buffer size
            char *new_ptr = (char *)realloc(*lineptr, *n);
            if (new_ptr == NULL) {
                return -1;  // Error reallocating memory
            }
            *lineptr = new_ptr;
        }

        if (c == '\n') {
            break;  // Reached end of line
        }
    }

    (*lineptr)[pos] = '\0';  // Null-terminate the string

    if (pos == 0 && c == EOF) {
        return -1;  // No characters read, reached end of file
    }

    return pos;  // Return the number of characters read
}

void remove_comments(char **commands) {
    for (int i = 0; commands[i] != NULL; i++) {
        char *hash_pos = strchr(commands[i], '#');
        if (hash_pos != NULL) {
            *hash_pos = '\0'; // Null-terminate the command at the '#' character
            if (hash_pos == commands[i]) {
                commands[i] = NULL; // If the entire command is a comment, remove it
            }
        }
    }
}