#include "main.h"

void check_semicollen(char **pcommands, char *name, int *status) {
    int i = 0;
    while (pcommands[i]) {
        char **command_group = NULL;
        int j = 0;

        // Collect commands until a semicolon is encountered
        while (pcommands[i] && strcmp(pcommands[i], ";") != 0) {
            command_group = realloc(command_group, (j + 2) * sizeof(char *));
            command_group[j] = strdup(pcommands[i]);
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
        while (pcommands[j] && strcmp(pcommands[j], "&&") != 0 && strcmp(pcommands[j], "||") != 0) {
            j++;
        }

        if (pcommands[j]) {
            if (strcmp(pcommands[j], "&&") == 0) {
                and_operator = 1;
            } else if (strcmp(pcommands[j], "||") == 0) {
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