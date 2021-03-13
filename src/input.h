#ifndef INPUT_H
#define INPUT_H

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

extern char *userInput;

typedef struct input {
    char** argv;
    char* command;
    size_t size;
} Input;


void *getUserInput(Input **input);
void freeInput(Input **input);

#endif