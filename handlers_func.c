#include "main.h"

/**
 * handle_cd - Handle the "cd" command
 * @commands: Array of command strings
 * @home_dir: Home directory path
 * @previous_dir: Pointer to the previous directory path
 *
 * This function handles the "cd" command, which is used to change the current directory.
 * It supports changing to the home directory, the previous directory, and a specified path.
 */
void handle_cd(char **commands, char *home_dir, char **previous_dir, char *name)
{
	if (commands[1] == NULL || _strcmp(commands[1], "~") == 0)
	{
		if (chdir(home_dir) == -1)
		{
			perror("cd");
		}
	}
	else if (_strcmp(commands[1], "-") == 0)
	{
		if (*previous_dir != NULL)
		{
			if (chdir(*previous_dir) == -1)
			{
				perror("cd");
			}
		}
	}
	else
	{
		if (chdir(commands[1]) == -1)
		{
			fprintf(stderr, "%s: 1: %s: can't cd to %s\n",name, commands[0],commands[1]);
		}
	}

	char *current_dir = getcwd(NULL, 0);
	if (current_dir != NULL)
	{
		setenv("PWD", current_dir, 1);
	}

	if (*previous_dir != NULL)
	{
		free(*previous_dir);
	}
	*previous_dir = current_dir;
}

/**
 * handle_echo - Handle the "echo" command with special variables
 * @commands: Array of command strings
 * @status: Pointer to the exit status
 *
 * This function handles the "echo" command, including special variables like "$?",
 * "$$", and "$PATH". It prints the corresponding values or error messages.
 */
void handle_echo(char **commands, int *status)
{
	if (_strcmp(commands[1], "$?") == 0)
	{
		printf("%d\n", *status);
	}
	else if (_strcmp(commands[1], "$$") == 0)
	{
		printf("%d\n", getpid());
	}
	else if (_strcmp(commands[1], "$PATH") == 0)
	{
		char *path_value = getenv("PATH");
		if (path_value != NULL)
		{
			printf("%s\n", path_value);
		}
		else
		{
			fprintf(stderr, "PATH environment variable not set\n");
		}
	}
	*status = 0;
}

/**
 * handle_environment - Handle environment-related commands (setenv and unsetenv)
 * @commands: Array of command strings
 * @status: Pointer to the exit status
 *
 * This function handles environment-related commands, including "setenv" and "unsetenv".
 * It sets or unsets environment variables based on the provided command and arguments.
 */
void handle_environment(char **commands, int *status)
{
	if (_strcmp(commands[0], "setenv") == 0)
	{
		if (commands[1] != NULL && commands[2] != NULL)
		{
			if (setenv(commands[1], commands[2], 1) != 0)
			{
				fprintf(stderr, "setenv: Failed to set environment variable\n");
			}
		}
		else
		{
			fprintf(stderr, "setenv: Invalid syntax\n");
		}
	}
	else if (_strcmp(commands[0], "unsetenv") == 0)
	{
		if (commands[1] != NULL)
		{
			if (unsetenv(commands[1]) != 0)
			{
				fprintf(stderr, "unsetenv: Failed to unset environment variable\n");
			}
		}
		else
		{
			fprintf(stderr, "unsetenv: Invalid syntax\n");
		}
	}
	*status = 0;
}

/**
 * cleanupFunction - Clean up and exit the program
 *
 * This function performs cleanup tasks before exiting the program, such as
 * releasing resources and performing any necessary finalization steps.
 * The program exits with the current exit status.
 */
