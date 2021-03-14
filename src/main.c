#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L

#include "input.h"
#include "exec.h"
#include "prompt.h"

static Input* input = NULL;

void initialise();
void terminate();

void handleSignal(int sig) {
}

int main() {
    initialise();

    char* test = malloc(100);
    test[0] = 10;

    // Main loop
    while (true) {
        printPrompt();
        getUserInput(&input);
        execute(input);
    }
}

/*
 * Initialisation
 */
void initialise() {
    atexit(&terminate);

    // Setting up SIGINT handler
    struct sigaction sa;
    sa.sa_handler = &handleSignal;
    sa.sa_flags = SA_RESTART;
    sigfillset(&sa.sa_mask);
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        fprintf(stderr, "Couldn't setup SIGINT handler: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

/*
 * Free any allocated memory and terminate the program
 */
void terminate() {
    if (userInput != NULL) free(userInput);
    freeInput(&input);
}
