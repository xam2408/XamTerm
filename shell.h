#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <linux/limits.h>

typedef struct default_commands {
    char *command;
    int (*com)(char **arg);
} default_commands_t;

int swdir(char **args);
int chngdir(char **arg);
int shell_exit(char **args);
int num_coms();

#endif
