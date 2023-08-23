#include "main.h"

/**
 * _strtok - Tokenizes a string based on delimiters.
 * @s: The input string.
 * @delim: Delimiter characters.
 *
 * Return: Pointer to the next token or NULL if no more tokens.
 */
char *_strtok(register char *s, register const char *delim)
{
	register char *spanp;
	register int c, sc;
	char *tok;
	static char *last;

	if (s == NULL && (s = last) == NULL)
		return (NULL);

cont:
	c = *s++;
	for (spanp = (char *)delim; (sc = *spanp++) != 0;)
    {
		if (c == sc)
			goto cont;
	}

	if (c == 0)
    {
		last = NULL;
		return (NULL);
	}
	tok = s - 1;

	for (;;)
    {
		c = *s++;
		spanp = (char *)delim;
		do {
			if ((sc = *spanp++) == c)
            {
				if (c == 0)
					s = NULL;
				else
					s[-1] = 0;
				last = s;
				return (tok);
			}
		} while (sc != 0);
	}
}

/**
 * filter - Tokenizes a string and filters commands.
 * @program_name: Name of the program.
 * @str: Input string containing commands.
 *
 * Return: Array of command strings.
 */
char **filter(const char *program_name, char *str)
{
	char *com, **commands = NULL;
	int i = 0;

	com = _strtok(str, " \n\t");
	while (com)
    {
		commands = realloc(commands, (i + 1) * sizeof(char *));
		if (!commands)
        {
			perror("realloc failed");
			exit(98);
		}
		commands[i] = com;
		i++;
		com = _strtok(NULL, " \t\n");
	}
	commands = realloc(commands, (i + 1) * sizeof(char *));
	if (!commands)
    {
		perror("realloc failed");
		exit(98);
	}
	commands[i] = NULL;

	for (i = 0; commands[i]; i++)
    {
		if (commands[i] && commands[i + 1])
        {
			if (_strcmp(commands[i], "exit") == 0)
            {
				char *endptr;
				int x = strtol(commands[i + 1], &endptr, 10);
				if (*endptr != '\0')
                {
					fprintf(stderr, "%s: 1: exit: Illegal number: %s\n",
							program_name, commands[i + 1]);
					_free(commands);
					free(commands);
					exit(2);
				} else if (x <= 0)
                {
					fprintf(stderr, "%s: 1: exit: Illegal number: %d\n",
							program_name, x);
					_free(commands);
					free(commands);
					exit(2);
				} else if (x >= 255)
                {
					_free(commands);
					free(commands);
					exit(232);
				}

				_free(commands);
				free(commands);
				exit((int)x);
			}
		} else if (_strcmp(commands[i], "exit") == 0)
        {
			_free(commands);
			free(commands);
			exit(0);
		}
	}

	commands[i] = NULL;
	return commands;
}