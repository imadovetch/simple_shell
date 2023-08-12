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

char **filter(const char *program_name,char *str ) {// ls                   \n       -a   
    char *com, **comands = NULL;
    int i = 0;

    com = _strtok(str, " \n\t");
    while (com) {
        comands = realloc(comands, (i + 1) * sizeof(char *));
        if (!comands) {
            perror("realloc failed");
            exit(98);
        }
        comands[i] = com;
        i++;
        com = _strtok(NULL, " \t\n");
    }
    comands = realloc(comands, (i + 1) * sizeof(char *));
    if (!comands) {
        perror("realloc failed");
        exit(98);
    }
	comands[i] = NULL;
	for(i = 0 ; comands[i]; i++)
	{
		if (comands[i] && comands[i+1]) {
    if (strcmp(comands[i], "exit") == 0) {
        char *endptr;
        int x = strtol(comands[i+1], &endptr, 10);
        //dir strtol dyalek
        if (*endptr != '\0') {
            // Conversion failed, it's not a valid number
            fprintf(stderr, "%s: 1: exit: Illegal number: %s\n", program_name, comands[i+1]);
            exit(2);
        } else if (x <= 0) {
            // Number is out of valid exit status range
            fprintf(stderr, "%s: 1: exit: Illegal number: %d\n", program_name, x);
            exit(2);
        }else if(x >= 255)
        {
            
            exit(232);
        }
        
        exit((int)x);
    }}
		
		
		else if (strcmp(comands[i],"exit") == 0)
		{
			exit(0);
		}
		
	}
    comands[i] = NULL;
    
    return (comands);//ls -a

}
