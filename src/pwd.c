#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

int main(int argc, char *argv[]) {
    int logical = 1;
    
    int opt;
    while ((opt = getopt(argc, argv, "LP")) != -1) {
        switch (opt) {
            case 'L': logical = 1; break;
            case 'P': logical = 0; break;
            default: exit(EXIT_FAILURE);
        }
    }

    char cwd[4096];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
        return 0;
    } else {
        perror("pwd");
        return 1;
    }
}
