#define _GNU_SOURCE

#include <signal.h>
#include <stdbool.h>
#include "input.h"
#include "exec.h"

char* promptPrefix = "# ";

void initialise();
void terminate();

void handleSignal(int sig) {
}

int main(int argc, char** argv) {
    initialise();
    char* input = NULL;
    int status;

    // Main loop
    while (true) {
        printf("%s", promptPrefix);

        input = getUserInput();
        if (input == NULL) terminate();

        execute(input);
    }

    terminate();
}

/*
 * Initialisation
 */
void initialise() {
    // Setting up SIGINT handler
    struct sigaction sa;
    sa.sa_handler = &handleSignal;
    sa.sa_flags = SA_RESTART;
    sigfillset(&sa.sa_mask);
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        fprintf(stderr, "Couldn't setup SIGINT handler: %s\n", strerror(errno));
    }
}

/*
 * Free any allocated memory and terminate the program
 */
void terminate() {
    if (userInput != NULL) free(userInput);

    printf("Goobye...\n");

    exit(EXIT_SUCCESS);
}