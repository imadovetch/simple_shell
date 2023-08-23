#include "main.h"

int status = 0;

void cleanupFunction()
{
	exit(status);
}
int handlers(char **commands, char *home_dir, char **previous_dir, char *program_name)
{
    if (_strcmp(commands[0], "cd") == 0)
    {
        handle_cd(commands, home_dir, previous_dir, program_name);
        return 1;
    }
    else if (_strcmp(commands[0], "echo") == 0)
    {
        handle_echo(commands, &status);
        return 1;
    }
    else if (_strcmp(commands[0], "setenv") == 0 || _strcmp(commands[0], "unsetenv") == 0)
    {
        handle_environment(commands, &status);
        return 1;
    }
    return 0;
}
int main(int argc, char **argv)
{
	int y,line_number = 0;
	extern char **environ;
	char *prompt = "mine$ ";
	char *buffer = NULL;
	size_t len,size = 0;
	int x = 0;
	char **commands = NULL;
	char *path = "/usr/bin";
	char *home_dir = getenv("HOME");
	char *previous_dir = NULL;
	int h,i;
	pid_t pid;
	char *line;
	while (1)
	{
		if (isatty(STDIN_FILENO) != 0)
			printf("%s", prompt);
		if (argc > 1)
		{
			FILE *file = fopen(argv[1], "r");
			if (file == NULL)
			{
				fprintf(stderr, "%s: %d: Can't open %s\n", argv[0], line_number, argv[1]);
				return (127);
			}
			
			line = NULL;
			len = 0;
			while (getline(&line, &len, file) != -1)
			{
				size_t line_len = my_strlen(line);
				if (line_len > 0 && line[line_len - 1] == '\n')
					line[line_len - 1] = '\0';
				commands = filter(argv[0], line);
				if (commands[0] != NULL)
				{
                    if (handlers(commands, home_dir, &previous_dir, argv[0]))
                        continue;
				}
			}
			free(line);
			fclose(file);
			return (status);
		}

		x = getline(&buffer, &size, stdin);
		line_number++;
		if (x == -1)
		{
			free(buffer);
			return (0);
		}
		len = my_strlen(buffer);
		if (len > 0 && buffer[len - 1] == '\n')
		{
			buffer[len - 1] = '\0';
		}
		for (i = 0; buffer[i]; i++)
		{
			if (!isspace(buffer[i]))
				break;
			if (buffer[i + 1] == '\0')
				exit(0);
		}

		y = 0;
		commands = filter(argv[0], buffer);

		for (h = 0; commands[h]; h++)
		{
			if (_strcmp(commands[h], "&&") == 0 || _strcmp(commands[h], "||") == 0)
			{
				check_opperators(commands, argv[0], &status);
				y = 1;
			}
		}

		for (h = 0; commands[h]; h++)
		{
			if (_strcmp(commands[h], ";") == 0)
			{
				check_semicollen(commands, argv[0], &status);
				y = 1;
			}
		}
		if (y == 1)
			continue;
		if (commands[0] != NULL && _strcmp(commands[0], "#") == 0 && isatty(STDIN_FILENO) == 0)
		{
			free(commands);
			free(buffer);
			exit(0);
		}
		remove_comments(commands);
		if (commands[0] != NULL)
		{
            if (handlers(commands, home_dir, &previous_dir, argv[0]))
                continue;
		}
		
		pid = fork();
		if (pid == 0)
		{
			char *command = commands[0];
			char *full_path = NULL;

			if (strchr(command, '/') == NULL)
				full_path = construct_full_path(argv[0], line_number, path, command);
			if (full_path == NULL)
				full_path = command;
			if (execve(full_path, commands, environ) == -1)
			{
				if (errno == ENOENT)
				{
					fprintf(stderr, "%s: %d: %s: not found\n", argv[0], line_number, command);
					status = 127;
				}
				else if (errno == EACCES)
				{
					fprintf(stderr, "sh: %s: permission denied\n", command);
					status = 2;
				}
				else
				{
					perror("execve");
					status = 1;
				}
			}
		}
		else if (pid > 0)
		{
			wait(&status);
			if (WIFEXITED(status))
				status = WEXITSTATUS(status);
			else
				status = 1;
		}
		else
		{
			perror("fork");
			status = 1;
		}

		if (isatty(STDIN_FILENO) == 0)
			atexit(cleanupFunction);
		free(commands);
	}
	_free(commands);
	free(commands);
	free(buffer);
	exit(status);
	return (0);
}
