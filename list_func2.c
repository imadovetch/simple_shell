#include "main.h"
extern char **environ;
/**
 * c_execvp - Locate and execute an executable using PATH
 * @file: Name of the executable file
 * @argv: Array of arguments for the executable
 *
 * This function locates the specified executable using the PATH environment
 * variable and attempts to execute it with the provided arguments using the
 * execve system call. It iterates through the directories in the PATH to find
 * the executable.
 *
 * Return: Return value of the execve system call, or -1 on failure.
 */
int c_execvp(const char *file, char *const argv[])
{
	/* Locate the executable using PATH environment variable */
	char *path = getenv("PATH");
	if (path == NULL)
	{
		perror("Error getting PATH");
		return -1;
	}

	char *path_copy = _strdup(path);
	if (path_copy == NULL)
	{
		perror("Error duplicating PATH");
		return -1;
	}

	int ret = -1;
	char *dir = strtok(path_copy, ":");
	while (dir != NULL)
	{
		/* Construct full path to executable */
		char full_path[1024];
		snprintf(full_path, sizeof(full_path), "%s/%s", dir, file);

		/* Attempt to execute the program */
		ret = execve(full_path, argv, environ);
		if (ret != -1)
		{
			break; /* Execution succeeded */
		}

		dir = strtok(NULL, ":");
	}

	free(path_copy);
	return ret;
}

/**
 * execute_command_group - Execute a group of commands as a child process
 * @command_group: Array of command strings to execute
 * @name: Name of the program
 * @status: Pointer to store the exit status
 *
 * This function forks a child process and executes a group of commands from
 * the specified array using the execute_command function. It waits for the
 * child process to complete and stores the exit status in the variable pointed
 * to by @status.
 */
void execute_command_group(char **command_group, char *name, int *status)
{
	pid_t pid = fork();
	if (pid == 0)
	{
		/* Child process */
		for (int i = 0; command_group[i] != NULL; i++)
		{
			execute_command(name, command_group[i], status);
		}
		exit(0); /* Exit the child process */
	}
	else if (pid > 0)
	{
		waitpid(pid, status, 0); /* Wait for the child process and store the exit status */
	}
	else
	{
		perror("fork");
	}
}

/**
 * check_semicollen - Check and execute groups of commands separated by semicolons
 * @pcommands: Array of command strings and semicolons
 * @name: Name of the program
 * @status: Pointer to store the exit status
 *
 * This function processes an array of command strings and semicolons, grouping
 * and executing commands that are separated by semicolons. It uses the execute_command_group
 * function to execute each group of commands. The function iterates through the array
 * and collects commands until a semicolon is encountered, then executes the collected
 * commands as a group.
 */
void check_semicollen(char **pcommands, char *name, int *status)
{
	int i = 0;
	while (pcommands[i])
	{
		char **command_group = NULL;
		int j = 0;

		/* Collect commands until a semicolon is encountered */
		while (pcommands[i] && _strcmp(pcommands[i], ";") != 0)
		{
			command_group = realloc(command_group, (j + 2) * sizeof(char *));
			command_group[j] = _strdup(pcommands[i]);
			command_group[j + 1] = NULL;
			i++;
			j++;
		}
		
		if (j > 0)
		{
			execute_command_group(command_group, name, status); /* Execute the group of commands */
		}

		/* Free memory for the command group */
		for (int k = 0; k < j; k++)
		{
			free(command_group[k]);
		}
		free(command_group);

		i++; /* Skip the semicolon if present */
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