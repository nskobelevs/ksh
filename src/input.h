#ifndef INPUT_H
#define INPUT_H

#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

extern char *userInput;

char *getUserInput(void);

#endif