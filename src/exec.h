#ifndef EXEC_H
#define EXEC_H

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>
#include <stdbool.h>
#include "input.h"

void execute(Input *const input);

#endif
