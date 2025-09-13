#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "cd: missing operand\n");
        return EXIT_FAILURE;
    }

    char *path = argv[1];
    char *home = getenv("HOME");
    char actual_path[1024];

    if (strcmp(path, "-") == 0) {
        char *oldpwd = getenv("OLDPWD");
        if (!oldpwd) {
            fprintf(stderr, "cd: OLDPWD not set\n");
            return EXIT_FAILURE;
        }
        path = oldpwd;
        printf("%s\n", path);
    } else if (path[0] == '~' && home) {
        if (path[1] == '\0' || path[1] == '/') {
            snprintf(actual_path, sizeof(actual_path), "%s%s", home, path + 1);
            path = actual_path;
        }
    }

    char *pwd = getenv("PWD");
    if (pwd) {
        setenv("OLDPWD", pwd, 1);
    }

    if (chdir(path) != 0) {
        fprintf(stderr, "cd: %s: %s\n", path, strerror(errno));
        return EXIT_FAILURE;
    }

    if (getcwd(actual_path, sizeof(actual_path)) != NULL) {
        setenv("PWD", actual_path, 1);
    }

    return EXIT_SUCCESS;
}
