#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <linux/limits.h>

int swdir(char **args);
int chngdir(char **arg);
int shell_exit(char **args);

typedef struct default_commands {
    char *command;
    int (*com)(char **arg);
} default_commands_t;

default_commands_t coms[] = {
    { "swdir", &swdir },
    { "chdir", &chngdir },
    { "exit", &shell_exit }
};

int num_coms() {
    return sizeof(coms) / sizeof(default_commands_t);
}

int swdir(char **args) {
    char dir_[PATH_MAX];
    if (getcwd(dir_, sizeof(dir_)) != NULL) {
        printf("%s\n", dir_);
    } else {
        perror("XamShell");
    }
    return 1;
}

int chngdir(char **arg) {
    if (arg[1] == NULL) {
        fprintf(stderr, "No directory provided !!!\n");
    } else {
        if (chdir(arg[1]) != 0) {
            fprintf(stderr, "Changing directory failed !!!\n");
        }
    }
    return 1;
}

int shell_exit(char **args) {
    return 0;
}

void handle_sigint(int sig) {
    printf("\n");
    printf("XamShell $ > ");
    fflush(stdout);
}

int main(void) {
    char *line = NULL;
    char *args[64];
    char *token;
    char usr = '$';
    size_t len = 0;
    ssize_t nread;
    int status = 1;

    signal(SIGINT, handle_sigint);

    do {
        printf("XamShell %c > ", usr);
        fflush(stdout);

        nread = getline(&line, &len, stdin);
        if (nread == -1) {
            printf("\n");
            break;
        }

        line[strcspn(line, "\n")] = '\0';

        int i = 0;
        token = strtok(line, " \t\r\n\a");
        while (token != NULL && i < (sizeof(args) / sizeof(char*) - 1)) {
            args[i++] = token;
            token = strtok(NULL, " \t\r\n\a");
        }
        args[i] = NULL;

        if (args[0] == NULL) {
            continue;
        }

        int command_found = 0;
        for (i = 0; i < num_coms(); i++) {
            if (strcmp(args[0], coms[i].command) == 0) {
                status = coms[i].com(args);
                command_found = 1;
                break;
            }
        }

        if (!command_found) {
            printf("XamShell: command not found: %s\n", args[0]);
        }

    } while (status);

    free(line);
    printf("Exiting XamShell.\n");
    return 0;
}
