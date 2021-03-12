#define _GNU_SOURCE

#include <signal.h>
#include <stdbool.h>
#include "input.h"
#include "exec.h"

char* promptPrefix = "# ";
static Input* input = NULL;

void initialise();
void terminate();

void handleSignal(int sig) {
}

int main(int argc, char** argv) {
    initialise();

    // Main loop
    while (true) {
        printf("%s ", promptPrefix);
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
    exit(EXIT_SUCCESS);
}