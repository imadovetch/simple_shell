#ifndef MAIN_H
#define MAIN_H

#define BUFFER_SIZE 2048
/* libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdarg.h>
#include <errno.h>
#include <stddef.h>

/* protos */
char *_strtok(s, delim);
char **filter(const char *program_name,char *str );
int _snprintf(char *str, size_t size, const char *format, ...);
#endif