#include "input.h"

char* userInput;
static size_t len = 0;

/*
 * Reads user input and returns a (malloc'd) pointer to it
 * Handles errors by telling the user if something went wrong.
 * Returns pointer to empty string if the line couldn't be read
 * Returns NULL on EOF input.
 */
char *getUserInput() {

    int status = getline(&userInput, &len, stdin);

    if (status == -1) {
        if (feof(stdin)) {
            return NULL;
        } else {
            fprintf(stderr, "Error reading user input: %s\n", strerror(errno));
            if (len > 0) userInput[0] = '\0';
        }
    }

    return userInput;
}