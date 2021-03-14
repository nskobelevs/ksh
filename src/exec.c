#include "exec.h"

static void cd(const Input *const input);

/*
 * Executes the given input's command in a child process
 */
void execute(Input *const input) {

    // return if invalid input
    // return is argv is null (no command to be ran)
    if (input == NULL || input->argv == NULL) return;

    // Looping from the end of the array until we find a ">"
    for (int index = input->size - 1; index >= 0; index--) {
        if (strcmp(input->argv[index], ">") == 0) {
            if (input->size == index + 1) {
                fprintf(stderr, "ksh: no redirect file specified\n");
                return;
            }
            char* filename = input->argv[index + 1];
            int redirectFile = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);

            if (redirectFile == -1) {
                fprintf(stderr, "ksh: error creating redirect file: %s (os error %d)\n", strerror(errno), strerror);
                return;
            } else {
                // Replaceing the argv from ">" to NULL to mark the end
                // And setting the new size appropriately to call execute recursively
                // This allowing chaining >
                // echo "test" > a > b > c
                input->argv[index] = NULL;
                input->size = index;

                int savedStdout = dup(STDOUT_FILENO);
                dup2(redirectFile, STDOUT_FILENO);

                execute(input);
                
                fflush(stdout);

                dup2(savedStdout, STDOUT_FILENO);
                close(redirectFile);
                return;
            }

        }
    }

    if (strcmp(input->command, "cd") == 0) {
        cd(input);
        return;
    } else if (strcmp(input->command, "exit") == 0) {
        exit(EXIT_SUCCESS);
    }

    // If the last arg is a "&" string, dont wait for the process to finish
    bool waitForChild = true;
    if (input->size > 1 && strcmp(input->argv[input->size - 1], "&") == 0) {
        waitForChild = false;
        input->argv[input->size - 1] = NULL;
    }

    int pid = fork();

    if (pid < 0) { // fork() failed

        // Relying on system() syscall if fork() failed
        int systemStatus = system(userInput);

        if (systemStatus == -1) {
            fprintf(stderr, "ksh: failed running system(): %s (os error %d)\n", strerror(errno), errno);
        }
    } else if (pid > 0) { // main process
        if (waitForChild) {
            int status;
            waitpid(pid, &status, 0); // wait for that specific child to finish
        } else {
            // Even if not waiting for child, sleep a bit not to print to the console too fast
            struct timespec sleepTime = {.tv_sec = 0, .tv_nsec = 25000000};
            nanosleep(&sleepTime, NULL);
        }
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

    char *home = getenv("HOME");

    char *dir = input->argv[1];

    if (input->size > 2) {
        fprintf(stderr, "ksh: cd: too many arguments\n");
        return;
    } else if (dir == NULL) {
        if (home == NULL) { // no arguments and home is not defined
            fprintf(stderr, "ksh: cd: \"HOME\" environment variable not set\n");
            return;
        } else {
            dir = home;
        }
    }

    char *newDir = NULL;
    if (strncmp(dir, "~/", 2) == 0 && home != NULL) {
        // Length of home + len of original dir - 1 (tilde char) + 1 (terminting null char)
        newDir = (char *)malloc((strlen(home) + strlen(dir)) * sizeof(char));
        strcpy(newDir, home);
        strcat(newDir, dir + 1);
        dir = newDir;
    }

    int chdirStatus = chdir(dir);

    if (newDir != NULL) free(newDir);

    if (chdirStatus == -1) {
        switch (errno) {
            case ENOENT:
                fprintf(stderr, "cd: no such file or directory: %s\n", dir);
                break;
            case EACCES:
                fprintf(stderr, "cd: permission denied: %s\n", dir);
                break;
            default:
                fprintf(stderr, "cd: %s (os error %d)\n", strerror(errno), errno);
                break;
        }
    }
}