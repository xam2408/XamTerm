#include "shell.h"

extern default_commands_t coms[];

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
