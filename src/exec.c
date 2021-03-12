#include "exec.h"

/*
 * Executes the given input's command in a child process
 */
void execute(const Input *const input) {

    // return if invalid input
    // return is argv is null (no command to be ran)
    if (input == NULL || input->argv == NULL) return;

    int pid = fork();

    if (pid < 0) { // fork() failed

        // Relying on system() syscall if fork() failed
        int systemStatus = system(userInput);

        if (systemStatus == -1) {
            fprintf(stderr, "ksh: failed running system(): %s (os error %d)\n", strerror(errno), errno);
        }
    } else if (pid > 0) { // main process
        int status;
        waitpid(pid, &status, 0); // wait for that specific child to finish
    } else { // child process
        int execStatus = execvp(input->command, input->argv); // run command

        if (execStatus == -1) {
            if (errno == ENOENT) {
                printf("ksh: command not found: %s\n", input->command);
            } else {
                fprintf(stderr, "ksh: error running command: %s (os error %d)\n", strerror(errno), errno);
            }

            exit(EXIT_FAILURE); // kill child if execution failed
        }
    }
}