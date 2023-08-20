#include "main.h"


char *_strtok(s, delim)
	register char *s;
	register const char *delim;
{
	register char *spanp;
	register int c, sc;
	char *tok;
	static char *last;


	if (s == NULL && (s = last) == NULL)
		return (NULL);

cont:
	c = *s++;
	for (spanp = (char *)delim; (sc = *spanp++) != 0;) {
		if (c == sc)
			goto cont;
	}

	if (c == 0) {		
		last = NULL;
		return (NULL);
	}
	tok = s - 1;

	for (;;) {
		c = *s++;
		spanp = (char *)delim;
		do {
			if ((sc = *spanp++) == c) {
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
char **filter(const char *program_name, char *str) {
    char *com, **commands = NULL;
    int i = 0;

    com = _strtok(str, " \n\t");
    while (com) {
        commands = realloc(commands, (i + 1) * sizeof(char *));
        if (!commands) {
            perror("realloc failed");
            exit(98);
        }
        commands[i] = com;
        i++;
        com = _strtok(NULL, " \t\n");
    }
    commands = realloc(commands, (i + 1) * sizeof(char *));
    if (!commands) {
        perror("realloc failed");
        exit(98);
    }
    commands[i] = NULL;

    for (i = 0; commands[i]; i++) {
        if (commands[i] && commands[i + 1]) {
            if (strcmp(commands[i], "exit") == 0) {
                char *endptr;
                int x = strtol(commands[i + 1], &endptr, 10);
                if (*endptr != '\0') {
                    fprintf(stderr, "%s: 1: exit: Illegal number: %s\n", program_name, commands[i + 1]);
                    _free(commands);free(commands);
                    exit(2);
                } else if (x <= 0) {
                    fprintf(stderr, "%s: 1: exit: Illegal number: %d\n", program_name, x);
                    _free(commands);free(commands);
                    exit(2);
                } else if (x >= 255) {
                    _free(commands);free(commands);
                    exit(232);
                }

                _free(commands);free(commands);
                exit((int)x);
            }
        } else if (strcmp(commands[i], "exit") == 0) {
            _free(commands);free(commands);
            exit(0);
        }
    }

    commands[i] = NULL;
    return commands;
}
int _free(char **p)
{
	if (p && *p)
	{
		free(*p);
		*p = NULL;
		return (1);
	}
	return (0);
}