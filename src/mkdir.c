#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>

int parents = 0;
int verbose = 0;
int mode = 0755;

int main(int argc, char *argv[]) {
    static struct option long_options[] = {
        {"parents", no_argument, 0, 'p'},
        {"verbose", no_argument, 0, 'v'},
        {"mode", required_argument, 0, 'm'},
        {0, 0, 0, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "pvm:", long_options, NULL)) != -1) {
        switch (opt) {
            case 'p': parents = 1; break;
            case 'v': verbose = 1; break;
            case 'm': mode = strtol(optarg, NULL, 8); break;
            default: exit(EXIT_FAILURE);
        }
    }

    if (optind >= argc) {
        fprintf(stderr, "Usage: %s [OPTION]... DIRECTORY...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    for (int i = optind; i < argc; i++) {
        if (parents) {
            char *path = argv[i];
            char *slash = path;
            
            while (*slash == '/') slash++;
            
            while ((slash = strchr(slash, '/')) != NULL) {
                *slash = '\0';
                if (mkdir(path, mode) != 0 && errno != EEXIST) {
                    perror("mkdir");
                    exit(EXIT_FAILURE);
                }
                *slash = '/';
                slash++;
                while (*slash == '/') slash++;
            }
        }
        
        if (mkdir(argv[i], mode) != 0) {
            if (errno == EEXIST && parents) {
                continue;
            }
            perror("mkdir");
            exit(EXIT_FAILURE);
        }

        if (verbose) {
            printf("mkdir: created directory '%s'\n", argv[i]);
        }
    }

    return EXIT_SUCCESS;
}
