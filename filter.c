#include "main.h"

char **filter(char *str) {// ls                   \n       -a   
    char *com, **comands = NULL;
    int i = 0;

    com = strtok(str, " \n\t");
    while (com) {
        comands = realloc(comands, (i + 1) * sizeof(char *));
        if (!comands) {
            perror("realloc failed");
            exit(98);
        }
        comands[i] = com;
        i++;
        com = strtok(NULL, " \t\n");
    }
    comands = realloc(comands, (i + 1) * sizeof(char *));
    if (!comands) {
        perror("realloc failed");
        exit(98);
    }
    comands[i] = NULL;
    
    return (comands);//ls -a

}
