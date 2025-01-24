#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "header.h"

int main() {
    for (int i = 0; i < 5; i++) { // Create 5 clients
        pid_t pid = fork();

        if (pid == 0) { // Child process
            srand(getpid()); // Seed the random number generator
            client_process();
            exit(0); // Exit child process after orders are completed
        } else if (pid < 0) { // Error in fork
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }
    }

    // Parent process waits for all child processes to finish
    for (int i = 0; i < 5; i++) {
        wait(NULL);
    }

    return 0;
}
