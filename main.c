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
	
	while((x = getline(&buffer, &size, stdin)) != -1 )
	{
		//rm_nl(buffer);

		
		if(strcmp(buffer, "exit") == 0)
		{
			printf("you out :/\n");
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
				if (strchr(comands[0], '/') == NULL) {
        char *path = getenv("PATH");
        char *dir = strtok(path, ":");

        while (dir != NULL) {
            char full_path[1024];
            snprintf(full_path, sizeof(full_path), "%s/%s", dir, comands[0]);

            if (access(full_path, 1) == 0) {
                return execve(full_path, comands,NULL);//
            }

            dir = strtok(NULL, ":");
        }
		if (execve(comands[0], comands,NULL) == -1) {
                    perror("execvp"); // Print an error if the command couldn't be executed
                }
		exit(EXIT_FAILURE);
        perror("Error: command not found in PATH");
        return -1;
    	}
					
            } else if (pid > 0) {
                // Parent process
                wait(NULL); // Wait for the child process to finish
            } else {
                perror("fork"); // Print an error if forking failed
            }
		printf("%s", prompt);

	
	}
	for (int i = 0; comands[i-1]; i++) {
        free(comands[i]);
    }
    free(comands);
    free(buffer);
    return 0;

}
