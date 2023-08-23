#ifndef MAIN_H
#define MAIN_H

#define BUFFER_SIZE 2048
#define MIN(a, b) ((a) < (b) ? (a) : (b))
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
char *_strtok(register char *s, register const char *delim);
char **filter(const char *program_name, char *str);
int _snprintf(char *str, size_t size, const char *format, ...);
/*string functions protos*/
size_t my_strlen(const char *str);
void my_strcpy(char *dest, const char *src);
int _strcmp(char *s1, char *s2);
/* allocate and free memory functions protos*/
int _free(char **p);
void string_free(char **ptr);
void *my_realloc(void *ptr, size_t new_size);
char *_strdup(char *str);
void adjust_ls_command(char **command);
char *construct_full_path(const char *program_name, int line_number,
const char *dir, const char *command);
int c_execvp(const char *file, char *const argv[]);
void execute_command(char *program_name, char *command, int *status);
void execute_command_group(char **command_group, char *name, int *status);
void check_semicollen(char **pcommands, char *name, int *status);
void check_opperators(char **pcommands, char *name, int *status);
void handle_cd(char **commands, char *home_dir,
char **previous_dir, char *name);
void handle_echo(char **commands, int *status);
void handle_environment(char **commands, int *status);
void remove_comments(char **commands);
int isspace( int ch);
#endif
