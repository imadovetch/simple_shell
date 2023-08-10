#ifndef MAIN_H
#define MAIN_H

/* libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdarg.h>
#include <errno.h>
/* protos */

char **filter(char *str);
int _snprintf(char *str, size_t size, const char *format, ...);
#endif