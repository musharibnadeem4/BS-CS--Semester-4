#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    int pipe1[2], pipe2[2];
    pid_t child_pid;

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    if ((child_pid = fork()) == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        // Child process (C)
        close(pipe1[1]);  
        close(pipe2[0]);  

        while (1) {
            char message[100];
            ssize_t n = read(pipe1[0], message, sizeof(message));
            if (n <= 0) {
                perror("Read failed");
                break;
            }
            message[n] = '\0';

            if (strcmp(message, "exit\n") == 0) {
                break;  
            }

            printf("Child received: %s", message);

            // Respond to the parent
            printf("Child response: ");
            fgets(message, sizeof(message), stdin);
            write(pipe2[1], message, strlen(message));
        }

        close(pipe1[0]); 
        close(pipe2[1]);
    } else {
        // Parent process (P)
        close(pipe1[0]);  
        close(pipe2[1]);  

        while (1) {
            char message[100];

            // Send a message to the child
            printf("Parent message: ");
            fgets(message, sizeof(message), stdin);
            write(pipe1[1], message, strlen(message));

            if (strcmp(message, "exit\n") == 0) {
                break;  
            }

          
            ssize_t n = read(pipe2[0], message, sizeof(message));
            if (n <= 0) {
                perror("Read failed");
                break;
            }
            message[n] = '\0';

            printf("Parent received: %s", message);
        }

        close(pipe1[1]);  
        close(pipe2[0]);  
    }

    return 0;
}