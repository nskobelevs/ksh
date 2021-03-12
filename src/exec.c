#include "exec.h"

static void cd(const Input *const input);

/*
 * Executes the given input's command in a child process
 */
void execute(const Input *const input) {

    // return if invalid input
    // return is argv is null (no command to be ran)
    if (input == NULL || input->argv == NULL) return;

    if (strcmp(input->command, "cd") == 0) {
        cd(input);
        return;
    } else if (strcmp(input->command, "exit")) {
        exit(EXIT_SUCCESS);
    }

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
        }

        exit(EXIT_FAILURE); // kill child
    }
}

/*
 * Built in cd command for the shell
 * no args -> home
 * one arg -> cd into that folder
 * two or more -> "too many arguments" error
 */
static void cd(const Input *const input) {

    if (input->argv[2] != NULL) {
        fprintf(stderr, "ksh: cd: too many arguments\n");
    } else if (input->argv[1] == NULL) { // no arguments
        char *home = getenv("HOME");
        if (home == NULL) {
            fprintf(stderr, "ksh: cd: \"HOME\" environment variable not set\n");
            return;
        } else {
            input->argv[1] = home;
        }
    }

    int chdirStatus = chdir(input->argv[1]);

    if (chdirStatus == -1) {
        fprintf(stderr, "ksh: cd: %s (os error %d)\n", strerror(errno), errno);
    }
}