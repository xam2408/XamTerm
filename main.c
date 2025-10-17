#include "shell.h"

default_commands_t coms[] = {
    { "swdir", &swdir },
    { "chdir", &chngdir },
    { "exit", &shell_exit }
};

int num_coms() {
    return sizeof(coms) / sizeof(default_commands_t);
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
