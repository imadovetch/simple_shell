#include "main.h"

/**
 * adjust_ls_command - Adjusts the command to start with "ls"
 * @command: Pointer to the command string
 *
 * This function checks if the command contains "ls" and adjusts the command
 * pointer to point to the first occurrence of "ls" within the command.
 *
 * @command should point to a valid string.
 *  If the command does not contain "ls",
 * the pointer remains unchanged.
 */
void adjust_ls_command(char **command)
{
	if (*command != NULL && strstr(*command, "ls") != NULL)
	{
		char *ls_position = strstr(*command, "ls");
		*command = ls_position;
	}
}
/**
 * construct_full_path - Construct a full path for a command
 * @program_name: Name of the program
 * @line_number: Line number of the call
 * @dir: Directory to search for the command
 * @command: Command to construct a path for
 *
 * Return: A pointer to the dynamically allocated full path string,
 *  or NULL on failure.
 */
char *construct_full_path(const char *program_name,
int line_number, const char *dir, const char *command)
{
	size_t dir_len = my_strlen(dir);
	size_t command_len = my_strlen(command);
	size_t path_len = dir_len + 1 + command_len + 1;

	char *full_path = malloc(path_len);

	if (full_path == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	if (command[0] == '/' || strchr(command, '/') != NULL)
	{
		my_strcpy(full_path, command);
	}
	else
	{
		/* Construct a full path using the specified directory and command */
		my_strcpy(full_path, dir);
		full_path[dir_len] = '/';
		my_strcpy(full_path + dir_len + 1, command);
	}
	full_path[path_len - 1] = '\0';

	/* Check if the file exists */
	if (access(full_path, F_OK) == -1)
	{
		/* File does not exist */
		fprintf(stderr, "%s: %d: %s: not found\n",
		 program_name, line_number, command);
		free(full_path);
		exit(127);
	}
	return (full_path);
}
/**
 * _getline - Read a line from a file stream
 * @lineptr: Pointer to a buffer to store the line
 * @n: Pointer to the size of the buffer
 * @stream: File stream to read from
 *
 * Return: Number of characters read, -1 on error or end of file
 */
ssize_t _getline(char **lineptr, size_t *n, FILE *stream)
{
	if (*lineptr == NULL || *n == 0)
	{
		*n = 128;  /* Initial buffer size */
		*lineptr = (char *)malloc(*n);
		if (*lineptr == NULL)
		{
			return (-1);  /* Error allocating memory */
		}
	}

	size_t pos = 0;
	int c;

	while ((c = fgetc(stream)) != EOF)
	{
		(*lineptr)[pos++] = (char)c;

		if (pos >= *n - 1)
		{
			*n *= 2;  /* Double the buffer size */
			char *new_ptr = (char *)realloc(*lineptr, *n);

			if (new_ptr == NULL)
			{
				return (-1);  /* Error reallocating memory */
			}
			*lineptr = new_ptr;
		}
		if (c == '\n')
			break;  /* Reached end of line */
	}
	(*lineptr)[pos] = '\0';  /* Null-terminate the string */
	if (pos == 0 && c == EOF)
		return (-1);  /* No characters read, reached end of file */
	return (pos);  /* Return the number of characters read */
}

/**
 * remove_comments - Remove comments from an array of commands
 * @commands: Pointer to an array of command strings
 *
 * This function iterates through the array of command strings and removes
 * any comments by null-terminating the string at the '#' character. If a
 * line consists entirely of a comment,
 *  the corresponding command is set to NULL.
 */
void remove_comments(char **commands)
{
	for (int i = 0; commands[i] != NULL; i++)
	{
		char *hash_pos = strchr(commands[i], '#');

		if (hash_pos != NULL)
		{
			*hash_pos = '\0';
			if (hash_pos == commands[i])
			{
				commands[i] = NULL;
			}
		}
	}
}

/**
 * execute_command - Execute a command
 * @program_name: Name of the program
 * @command: Command to execute
 * @status: Pointer to store the exit status
 *
 * This function forks a child process and executes the specified command using
 * the execvp system call.It waits for the child process to complete and stores
 * the exit status in the variable pointed to by @status.
 */
void execute_command(char *program_name, char *command, int *status)
{
	pid_t pid = fork();

	if (pid == 0)
	{
		/* Child process */
		char *args[] = {command, NULL};

		c_execvp(command, args);

		/* If execvp returns, the command was not found */
		fprintf(stderr, "%s: %d: %s: not found\n", program_name, 1, command);
		exit(127); /* Exit with an appropriate status */
	}
	else if (pid > 0)
	{
		waitpid(pid, status, 0);
	}
	else
	{
		perror("fork");
	}
}
