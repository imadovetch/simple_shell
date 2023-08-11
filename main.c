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
/*
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
        exit(0);
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
    int status = 0;
      int last_status = 0;
    while (1) {
        
		if(isatty(STDIN_FILENO) != 0)
			printf("%s", prompt);
        x = getline(&buffer, &size, stdin);
		
        if (x == -1) {
            //printf("\n");FREE
            return 0;
        }

        // Remove newline character
        size_t len = my_strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        

        commands = filter(buffer);

        if ((strcmp(commands[0], "exit") == 0) ) {
            //zid lfree
            if (commands[1] != NULL)
                status = atoi(commands[1]);
            else
                status = 0;
            //printf("%d\n",status);
            exit(0);
        }
        if (strcmp(commands[0], "echo") == 0 && strcmp(commands[1], "$?") == 0) {
        printf("%d\n", status);
        continue;; // Display the exit status of the last command
        }
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

// ...

if (execve(full_path, commands, environ) == -1) {
    char error_message[64];
    strcpy(error_message, "sh: 1: ");
    strcat(error_message, command);
    strcat(error_message, ": not found\n");
    write(STDERR_FILENO, error_message, strlen(error_message));
    exit(0);
    continue; // Continue to the next iteration of the loop
}



        } else if (pid > 0) {
            // Parent process
            wait(NULL); // Wait for the child process to finish
            last_status = status;
		} else {
            perror("fork"); // Print an error if forking failed
        }        
    }

   for (int i = 0; commands[i] != NULL; i++) {
    free(commands[i]);
}
free(commands);
free(buffer);
//exit(status);
    return status;
}
*/
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
        exit(0);
    }

    // If the command is an absolute path or contains any slashes, use it directly
    if (command[0] == '/' || strchr(command, '/') != NULL) {
        my_strcpy(full_path, command);
    } else {
        // Construct a full path using the specified directory and command
        my_strcpy(full_path, dir);
        full_path[dir_len] = '/';
        my_strcpy(full_path + dir_len + 1, command);
    }
    
    full_path[path_len - 1] = '\0';

    // Check if the file exists
    if (access(full_path, F_OK) == -1) {
        // File does not exist
        fprintf(stderr, "sh: %s: not found\n", command);
        free(full_path);
        exit(127);
    }

    return full_path;
}
int status = 0;
void cleanupFunction() {
    exit(status);
}

int main(int argc, char **argv) {
    (void)argc, (void)argv;
    char *prompt = "mine$ ";
    char *buffer = NULL;
    size_t size = 0;
    int x = 0;
    char **commands = NULL;
    char *path = "/usr/bin"; // Use your default path here
   // int status = 0;

    while (1) {
		if(isatty(STDIN_FILENO) != 0)
			printf("%s", prompt);
            
        x = getline(&buffer, &size, stdin);
		
        if (x == -1) {
            //printf("\n");FREE
            return 0;
        }

        // Remove newline character
        size_t len = my_strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        for (int i = 0; buffer[i]; i++) {
    if (!isspace(buffer[i])) {
        // Found a non-space character
        break;
    }
    
    // If we've reached the end of the buffer and all characters are spaces
    if (buffer[i + 1] == '\0') {
        
        exit(0);
    }
}
       
        commands = filter(buffer);
        //hada li lte7t 7al mo2a9at;
        /*if (commands[0] == NULL)
            exit(0);*/
        if (commands[0] != NULL) {
        if (strcmp(commands[0], "exit") == 0) {
        // zid lfree
        if (commands[1] != NULL)
            status = atoi(commands[1]);
        else
            status = 0;
        // printf("%d\n",status);
        break;
        //exit(status);
    }
        
        if (strcmp(commands[0], "echo") == 0 && strcmp(commands[1], "$?") == 0) {
        printf("%d\n", status);
        status = 0;
        continue;; // Display the exit status of the last command
        }
}
        

        if(isatty(STDIN_FILENO) == 0)
        {  
            pid_t pid = fork(); // Create a child process
            if(kill(pid, 0) != 0)
                printf("dkhel\n");
      
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
    
   if (execve(full_path, commands, environ) == -1) {
    if (errno == ENOENT) {
        fprintf(stderr, "sh: %s: not found\n", command);
        status = 127;
    } else if (errno == EACCES) {
        fprintf(stderr, "sh: %s: permission denied\n", command);
        status = 2;
    } else {
        perror("execve");
        status = 1; // Set an appropriate status for other errors
    }
    
   
}

} else if (pid > 0) {
    // Parent process
    wait(&status); // Wait for the child process to finish
    if (WIFEXITED(status)) {
        status = WEXITSTATUS(status); // Get the exit status of the child process
    } else {
        // Child process did not terminate normally
        status = 1; // Set an appropriate non-zero status
    }
} else {
    perror("fork"); // Print an error if forking failed
    status = 1; // Set an appropriate non-zero status
}
   atexit(cleanupFunction);}
             
 if(isatty(STDIN_FILENO) != 0){      
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
    
   if (execve(full_path, commands, environ) == -1) {
    if (errno == ENOENT) {
        fprintf(stderr, "sh: %s: not found\n", command);
        status = 127;
    } else if (errno == EACCES) {
        fprintf(stderr, "sh: %s: permission denied\n", command);
        status = 2;
    } else {
        perror("execve");
        status = 1; // Set an appropriate status for other errors
    }
    
    ; // Exit the child process with the updated status
}
if(isatty(STDIN_FILENO) == 0)
{
    printf("ana hmar\n");
}


} else if (pid > 0) {
    // Parent process
    wait(&status); // Wait for the child process to finish
    if (WIFEXITED(status)) {
        status = WEXITSTATUS(status); // Get the exit status of the child process
    } else {
        // Child process did not terminate normally
        status = 1; // Set an appropriate non-zero status
    }
} else {
    perror("fork"); // Print an error if forking failed
    status = 1; // Set an appropriate non-zero status
}
    }}

    for (int i = 0; commands[i] != NULL; i++) {
        free(commands[i]);
    }
    free(commands);
    //free(buffer);
    exit(status);
    return 0;
}//asel
// commantit comand == null