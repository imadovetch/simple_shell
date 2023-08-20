#include "main.h"

extern char **environ;



void adjust_ls_command(char **command) {
    if (*command != NULL && strstr(*command, "ls") != NULL) {
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

int status = 0;
void cleanupFunction() {
    exit(status);
}
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
void execute_command(char *program_name, char *command, int *status) {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        char *args[] = {command, NULL};
        c_execvp(command, args);

        // If execvp returns, the command was not found
        fprintf(stderr, "%s: %d: %s: not found\n", program_name, 1, command);
        exit(127); // Exit with an appropriate status
    } else if (pid > 0) {
        waitpid(pid, status, 0); // Wait for the child process and store the exit status
    } else {
        perror("fork");
    }
}
int c_execvp(const char *file, char *const argv[]) {
    // Locate the executable using PATH environment variable
    char *path = getenv("PATH");
    if (path == NULL) {
        perror("Error getting PATH");
        return -1;
    }

    char *path_copy = _strdup(path);
    if (path_copy == NULL) {
        perror("Error duplicating PATH");
        return -1;
    }

    int ret = -1;
    char *dir = strtok(path_copy, ":");
    while (dir != NULL) {
        // Construct full path to executable
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, file);

        // Attempt to execute the program
        ret = execve(full_path, argv, environ);
        if (ret != -1) {
            break;  // Execution succeeded
        }

        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return ret;
}
void execute_command_group(char **command_group, char *name, int *status) {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        for (int i = 0; command_group[i] != NULL; i++) {
            execute_command(name, command_group[i], status);
        }
        exit(0); // Exit the child process
    } else if (pid > 0) {
        waitpid(pid, status, 0); // Wait for the child process and store the exit status
    } else {
        perror("fork");
    }
}

void check_semicollen(char **pcommands, char *name, int *status) {
    int i = 0;
    while (pcommands[i]) {
        char **command_group = NULL;
        int j = 0;

        // Collect commands until a semicolon is encountered
        while (pcommands[i] && _strcmp(pcommands[i], ";") != 0) {
            command_group = realloc(command_group, (j + 2) * sizeof(char *));
            command_group[j] = _strdup(pcommands[i]);
            command_group[j + 1] = NULL;
            i++;
            j++;
        }
        
        if (j > 0) {
            execute_command_group(command_group, name, status); // Execute the group of commands
        }

        // Free memory for the command group
        for (int k = 0; k < j; k++) {
            free(command_group[k]);
        }
        free(command_group);

        i++; // Skip the semicolon if present
    }
}
void check_opperators(char **pcommands, char *name, int *status) {
    int i = 0;
    while (pcommands[i]) {
        int j = i;
        int and_operator = 0; // To track the AND operator
        int or_operator = 0;  // To track the OR operator

        // Find the next AND or OR operator
        while (pcommands[j] && _strcmp(pcommands[j], "&&") != 0 && _strcmp(pcommands[j], "||") != 0) {
            j++;
        }

        if (pcommands[j]) {
            if (_strcmp(pcommands[j], "&&") == 0) {
                and_operator = 1;
            } else if (_strcmp(pcommands[j], "||") == 0) {
                or_operator = 1;
            }

            pcommands[j] = NULL; // Null-terminate the first command

            // Execute the command
            execute_command(name, pcommands[i], status);

            if (*status == 0 && and_operator) {
                // Execute the next command only if the previous one succeeded
                execute_command(name, pcommands[j + 1], status);
            } else if (*status != 0 && or_operator) {
                // Execute the next command only if the previous one failed
                execute_command(name, pcommands[j + 1], status);
            }

            i = j + 2; // Skip the operator and the next command
        } else {
            // No more operators found, execute the last command
            execute_command(name, pcommands[i], status);
            break;
        }
    }
}
void handle_cd(char **commands, char *home_dir, char **previous_dir) {
    if (commands[1] == NULL || _strcmp(commands[1], "~") == 0) {
        if (chdir(home_dir) == -1) {
            perror("cd");
        }
    } else if (_strcmp(commands[1], "-") == 0) {
        if (*previous_dir != NULL) {
            if (chdir(*previous_dir) == -1) {
                perror("cd");
            }
        }
    } else {
        if (chdir(commands[1]) == -1) {
            fprintf(stderr, "cd: can't cd to %s\n", commands[1]);
        }
    }

    char *current_dir = getcwd(NULL, 0);
    if (current_dir != NULL) {
        setenv("PWD", current_dir, 1);
    }

    if (*previous_dir != NULL) {
        free(*previous_dir);
    }
    *previous_dir = current_dir;
}

void handle_echo(char **commands, int *status) {
    if (_strcmp(commands[1], "$?") == 0) {
        printf("%d\n", *status);
    } else if (_strcmp(commands[1], "$$") == 0) {
        printf("%d\n", getpid());
    } else if (_strcmp(commands[1], "$PATH") == 0) {
        char *path_value = getenv("PATH");
        if (path_value != NULL) {
            printf("%s\n", path_value);
        } else {
            fprintf(stderr, "PATH environment variable not set\n");
        }
    }
    *status = 0;
}

void handle_environment(char **commands, int *status) {
    if (_strcmp(commands[0], "setenv") == 0) {
        if (commands[1] != NULL && commands[2] != NULL) {
            if (setenv(commands[1], commands[2], 1) != 0) {
                fprintf(stderr, "setenv: Failed to set environment variable\n");
            }
        } else {
            fprintf(stderr, "setenv: Invalid syntax\n");
        }
    } else if (_strcmp(commands[0], "unsetenv") == 0) {
        if (commands[1] != NULL) {
            if (unsetenv(commands[1]) != 0) {
                fprintf(stderr, "unsetenv: Failed to unset environment variable\n");
            }
        } else {
            fprintf(stderr, "unsetenv: Invalid syntax\n");
        }
    }
    *status = 0;
}

int main(int argc, char **argv) {
   int line_number = 0;
    char *prompt = "mine$ ";
    char *buffer = NULL;
    size_t size = 0;
    int x = 0;
    char **commands = NULL;
    char *path = "/usr/bin";
    char *home_dir = getenv("HOME");
    char *previous_dir = NULL;
    char *current_dir = NULL;
    while (1) {
		if(isatty(STDIN_FILENO) != 0)
			printf("%s", prompt);
        
         if (argc > 1) {

        FILE *file = fopen(argv[1], "r");
if (file == NULL) {
    fprintf(stderr, "%s: %d: Can't open %s\n", argv[0], line_number, argv[1]);
    return 127;
}
        char *line = NULL;
        size_t len = 0;
                   while (_getline(&line, &len, file) != -1) {
               size_t line_len = my_strlen(line);
               if (line_len > 0 && line[line_len - 1] == '\n') {
                   line[line_len - 1] = '\0';
               }
               char **commands = filter(argv[0], line);

               if (commands[0] != NULL) {
            if (_strcmp(commands[0], "cd") == 0) {
                handle_cd(commands, home_dir, &previous_dir);
                continue;
            } else if (_strcmp(commands[0], "echo") == 0) {
                handle_echo(commands, &status);
                continue;
            } else if (_strcmp(commands[0], "setenv") == 0 || _strcmp(commands[0], "unsetenv") == 0) {
                handle_environment(commands, &status);
                continue;
            }
        }
           }
           free(line);
           fclose(file);
           return status;}
        x = _getline(&buffer, &size, stdin);  
		line_number++;
        if (x == -1) { 
            free(buffer);/*found it */
            return 0;
        }
 
        size_t len = my_strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        for (int i = 0; buffer[i]; i++) {
    if (!isspace(buffer[i])) {
        break;
    }
    if (buffer[i + 1] == '\0') {
        exit(0);
    }
}
       int y = 0;
        commands = filter(argv[0],buffer);
        for(int h = 0;commands[h];h++)
        {
            if (_strcmp(commands[h],"&&") == 0 || _strcmp(commands[h] , "||") == 0)
            {
                check_opperators(commands, argv[0], &status);
                y = 1;
            }
                    
        }
        for(int h = 0;commands[h];h++)
        {
            if (_strcmp(commands[h],";") == 0)
            {
                check_semicollen(commands, argv[0], &status);
                y=1;
            }       
        }
        if (y == 1)
            continue;
        
       if (commands[0] != NULL && _strcmp(commands[0], "#") == 0 && isatty(STDIN_FILENO) == 0) {
        free(commands);/*non tested*/
        free(buffer);/*free done */
    exit(0);
}
        remove_comments(commands);
        if (commands[0] != NULL) {
            if (_strcmp(commands[0], "cd") == 0) {
                handle_cd(commands, home_dir, &previous_dir);
                continue;
            } else if (_strcmp(commands[0], "echo") == 0) {
                handle_echo(commands, &status);
                continue;
            } else if (_strcmp(commands[0], "setenv") == 0 || _strcmp(commands[0], "unsetenv") == 0) {
                handle_environment(commands, &status);
                continue;
            }
        }      
       pid_t pid = fork();
      
if (pid == 0) {
    char *command = commands[0];
    char *full_path = NULL;

    if (strchr(command, '/') == NULL) {
        full_path = construct_full_path(argv[0],line_number , path, command);


    }

    if (full_path == NULL) {
        full_path = command;
    }
    
   if (execve(full_path, commands, environ) == -1) {
    if (errno == ENOENT) {
        fprintf(stderr, "%s: %d: %s: not found\n", argv[0], line_number, command);
        status = 127;
    } else if (errno == EACCES) {
        fprintf(stderr, "sh: %s: permission denied\n", command);
        status = 2;
    } else {
        perror("execve");
        status = 1; 
    }
    
    ; 
}
} else if (pid > 0) {
    wait(&status);
    if (WIFEXITED(status)) {
        status = WEXITSTATUS(status);
    } else {
        status = 1;
    }
} else {
    perror("fork");
    status = 1;
}
if(isatty(STDIN_FILENO) == 0)
        { atexit(cleanupFunction);}
        
    free(commands);
    }

    for (int i = 0; commands[i] != NULL; i++) {
        free(commands[i]);
    }
    free(commands);
    free(buffer);
    exit(status);
    return 0;
}