#include "input.h"

char* userInput;

static char** split(char * rawInput, size_t *len);
static Input *createInput();

/*
 * Splits a given string into an array of strings using a space as the delim 
 * returned pointer must be freed
 */
static char** split(char * rawInput, size_t *len) {
    char ** argv = (char **)malloc(sizeof(*argv));

    char *token = strtok(rawInput, " ");
    argv[0] = token;

    size_t count = 1;

    while (token != NULL) {
        count += 1;
        token = strtok(NULL, " ");

        argv = realloc(argv, count * sizeof(*argv));
        
        if (argv == NULL) {
            perror("realloc() failed\n");
            exit(EXIT_FAILURE);
        }

        argv[count - 1] = token;
    }

    *len = count;
    return argv;
}

/* 
 * Creates an Input instance and allocates required memory
 * Initialises fields to NULL
 * returned pointer must be freed
 */
static Input *createInput() {
    Input *input = (Input *)malloc(sizeof(*input));
    if (input == NULL) {
        fprintf(stderr, "ksh: malloc() failed: %s (os error %d)\n", strerror(errno), errno);
        exit(EXIT_FAILURE);
    }
    input->argv = NULL;
    input->command = NULL;
    input->size = 0;
    return input;
}

/*
 * Frees Input instance and it's fields
 */
void freeInput(Input **input) {
    if (input == NULL || (*input) == NULL) return;

    if ((*input)->argv != NULL) free((*input)->argv);
    free(*input);
}

/*
 * Reads user input and sets input fields appropriately
 * Will allocate memory for Input instance if a pointer to NULL is passed in
 * If line couldn't be read. Input fields argv and command will be NULL
 * terminates the program on EOF input
 */
void *getUserInput(Input **input) {

    if (input == NULL) {
        perror("ksh: internal error: getUserInput() received NULL as input (os error baddev)\n");
        exit(EXIT_FAILURE);
    }
    
    if ((*input) == NULL) { 
        (*input) = createInput();
    } else if ((*input)->argv != NULL) {
        free((*input)->argv); // free argv from previous call
        (*input)->argv = NULL; // set to NULL to mark is as freed
    }

    static size_t len = 0;
    int status = getline(&userInput, &len, stdin);

    if (status == -1) {
        if (feof(stdin)) {
            exit(EXIT_SUCCESS);
        } else {
            fprintf(stderr, "ksh: error reading input: %s (os error %d)\n", strerror(errno), errno);
            return;
        }
    }

    // remmoving trailling new line
    if (userInput[strlen(userInput) - 1] == '\n')
        userInput[strlen(userInput) - 1] = '\0';

    // empty input
    if (strlen(userInput) == 0) return;

    size_t argvCount;
    char** argv = split(userInput, &argvCount);

    (*input)->argv = argv;
    (*input)->command = argv[0];
    (*input)->size = argvCount - 1;
}