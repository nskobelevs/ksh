#include "exec.h"

/*
 * Executes the gives command in a child shell process
 */
void execute(char *input) {
    int status = system(input);

    if (status == -1) {
        fprintf(stderr, "Error running command: %s\n", strerror(errno));
    }
}