#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

int main(int argc, char *argv[]) {
    int ignore_fail = 0;
    int verbose = 0;
    
    static struct option long_options[] = {
        {"ignore-fail-on-non-empty", no_argument, 0, 0},
        {"verbose", no_argument, 0, 'v'},
        {0, 0, 0, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "pv", long_options, NULL)) != -1) {
        switch (opt) {
            case 'p': ignore_fail = 1; break;
            case 'v': verbose = 1; break;
            case 0: ignore_fail = 1; break;
            default: exit(EXIT_FAILURE);
        }
    }

    if (optind >= argc) {
        fprintf(stderr, "Usage: %s [OPTION]... DIRECTORY...\n", argv[0]);
        return 1;
    }

    for (int i = optind; i < argc; i++) {
        if (rmdir(argv[i]) != 0) {
            if (!ignore_fail) {
                perror("rmdir");
                return 1;
            }
        } else if (verbose) {
            printf("rmdir: removed directory '%s'\n", argv[i]);
        }
    }
    return 0;
}
