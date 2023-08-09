/*#include "main.h"

// ! size_t getline(char **lineptr, size_t *n,FILE *stream);
// ! size_t strspn(const char *s, const char *accept);
// ? size_t strcspn(const char *s, const char *reject);

int main(int argc, char **argv)
{
	(void)argc, (void)argv;
	char *prompt = "mine$ ";
	char *buffer = NULL;
	size_t size = 0;
	int x = 0;
	char **comands = NULL;
	printf("%s", prompt);
	while(1){
	while((x = getline(&buffer, &size, stdin)) != -1 )
	{
		rm_nl(buffer);

		
		if(strcmp(buffer, "exit") == 0)
		{
			printf("you out :/\n");
			free(comands);
			free(buffer);
			exit(0);
		}
		comands = filter(buffer);
		//printf("%s\n", buffer);
		//printf("%d\n", x);
		// printf("Len is  %i\n", strlen(buffer));
		
		//printf("pi%d\n",pid);
		//printf("ppi%d\n",ppid);
		
		pid_t pid = fork(); // Create a child process
            if (pid == 0) {
                // Child process
                if (execve(comands[0], comands,NULL) == -1) {
                    perror("execvp"); // Print an error if the command couldn't be executed
                }
                exit(EXIT_FAILURE);
            } else if (pid > 0) {
                // Parent process
                wait(NULL); // Wait for the child process to finish
            } else {
                perror("fork"); // Print an error if forking failed
            }
		
		printf("%s", prompt);
		
	}
	
	
	}	
}*/
/*
#include "libs.h"

void sigint_handler(int __attribute__((unused)) sig)
{
        write(1, "\n", 1);
		prompt();
		fflush(stdout);
}                                                            

void sigquit_handler(int __attribute__((unused)) sig)
{
        write(1, "\n", 1);
        exit(0);
}*/

#include "main.h"

extern char **environ;

size_t my_strlen(const char *str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

void my_strcpy(char *dest, const char *src) {
    while ((*dest = *src) != '\0') {
        dest++;
        src++;
    }
}



char *construct_full_path(const char *dir, const char *command) {
    size_t dir_len = my_strlen(dir);
    size_t command_len = my_strlen(command);
    size_t path_len = dir_len + 1 + command_len + 1;

    char *full_path = malloc(path_len);
    if (full_path == NULL) {
        perror("malloc");
        exit(1);
    }

    my_strcpy(full_path, dir);
    full_path[dir_len] = '/';
    my_strcpy(full_path + dir_len + 1, command);
    full_path[path_len - 1] = '\0';

    return full_path;
}

int main(int argc, char **argv) {
    (void)argc, (void)argv;
    char *prompt = "mine$ ";
    char *buffer = NULL;
    size_t size = 0;
    int x = 0;
    char **commands = NULL;
    char *path = "/usr/bin"; // Use your default path here
    printf("%s", prompt);

    while (1) {
        x = getline(&buffer, &size, stdin);
        if (x == -1) {
            printf("\n");
            return 0;
        }

        // Remove newline character
        /*size_t len = my_strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }*/

        if (strcmp(buffer, "exit") == 0) {
            printf("you're out :/\n");
            exit(0);
        }

        commands = filter(buffer);

        pid_t pid = fork(); // Create a child process
        if (pid == 0) {
            // Child process
            char *command = commands[0];
            char *full_path = NULL;

            if (strchr(command, '/') == NULL) {
                full_path = construct_full_path(path, command);
            }

            if (full_path == NULL) {
                full_path = command;
            }

            execve(full_path, commands, environ);
            perror("execve"); // Print error if execve fails
            exit(1);
        } else if (pid > 0) {
            // Parent process
            wait(NULL); // Wait for the child process to finish
        } else {
            perror("fork"); // Print an error if forking failed
        }

        printf("%s", prompt);
    }

    for (int i = 0; commands[i] != NULL; i++) {
        free(commands[i]);
    }
    free(commands);
    free(buffer);
    return 0;
}
