#define _GNU_SOURCE      // potrzebne w niektórych systemach
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <linux/limits.h>

//embedded functions

void swdir() { //show working directory
    char dir_[PATH_MAX];
    if (getcwd(dir_, sizeof(dir_)) != NULL) {
        printf("%s\n", dir_);
    } else {
        perror("XamShell");
    }
}


//handler for signal (ctrl+c)
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
    //char super_usr = '#';

    size_t len = 0;
    ssize_t nread;

    // Ustaw handler dla Ctrl+C
    signal(SIGINT, handle_sigint);

    while (1) {
        printf("XamShell %c > ", usr);
        fflush(stdout);

        nread = getline(&line, &len, stdin);
        if (nread == -1) {
            printf("\nExiting XamShell.\n");
            break;
        }

        line[strcspn(line, "\n")] = '\0'; //remove last endline char to get exit working

        int i = 0;
        token = strtok(line, " \t\r\n\a");
        while (token != NULL) {
            args[i] = token;
            i++;
            if (i >= sizeof(args) / sizeof(char*)) {
                break;
            }
            token = strtok(NULL, " \t\r\n\a");
        }
        args[i] = NULL;

        if (args[0] == NULL) {
            continue;
        }

        if (strncmp(args[0], "exit", sizeof("exit")) == 0) {
            printf("Exiting XamShell.\n");
            break;
        } 
        else if (strncmp(args[0], "swdir", sizeof("swdir")) == 0) {
            swdir();
        }
        else {
            printf("XamShell: command not found: %s\n", args[0]);
        }
    }

    free(line); // free getline alocated memory
    return 0;
}
