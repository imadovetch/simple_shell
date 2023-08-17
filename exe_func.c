#include "main.h"

void execute_command(char *program_name, char *command, int *status) {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        char *args[] = {command, NULL};
        execvp(command, args);

        // If execvp returns, the command was not found
        fprintf(stderr, "%s: %d: %s: not found\n", program_name, 1, command);
        exit(127); // Exit with an appropriate status
    } else if (pid > 0) {
        waitpid(pid, status, 0); // Wait for the child process and store the exit status
    } else {
        perror("fork");
    }
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