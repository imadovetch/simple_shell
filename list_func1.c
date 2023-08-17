#include "main.h"

void cleanupFunction() {
    exit(status);
}
/**
 * adjust_ls_command - Adjusts the command pointer to point to "ls" part
 * @command: Pointer to the command string
 */
void adjust_ls_command(char **command)
{
    if (*command != NULL && strstr(*command, "ls") != NULL)
    {
        char *ls_position = strstr(*command, "ls");
        *command = ls_position;
    }
}

char *construct_full_path(const char *program_name, int line_number, const char *dir, const char *command) {
    size_t dir_len = my_strlen(dir);
    size_t command_len = my_strlen(command);
    size_t path_len = dir_len + 1 + command_len + 1;

    char *full_path = malloc(path_len);
    if (full_path == NULL) {
        perror("malloc");
        exit(0);
    }

    // If the command is an absolute path or contains any slashes, use it directly
    if (command[0] == '/' || strchr(command, '/') != NULL) {
        my_strcpy(full_path, command);
    } else {
        // Construct a full path using the specified directory and command
        my_strcpy(full_path, dir);
        full_path[dir_len] = '/';
        my_strcpy(full_path + dir_len + 1, command);
    }
    
    full_path[path_len - 1] = '\0';

    // Check if the file exists
    if (access(full_path, F_OK) == -1) {
        // File does not exist
        fprintf(stderr, "%s: %d: %s: not found\n", program_name, line_number, command);
        free(full_path);
        exit(127);
    }

    return full_path;
}
