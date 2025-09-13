#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>

int force = 0;
int verbose = 0;

int main(int argc, char *argv[]) {
    static struct option long_options[] = {
        {"force", no_argument, 0, 'f'},
        {"verbose", no_argument, 0, 'v'},
        {0, 0, 0, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "fv", long_options, NULL)) != -1) {
        switch (opt) {
            case 'f': force = 1; break;
            case 'v': verbose = 1; break;
            default: exit(EXIT_FAILURE);
        }
    }

    if (argc - optind < 2) {
        fprintf(stderr, "Usage: %s [OPTION]... SOURCE... DIRECTORY\n", argv[0]);
        fprintf(stderr, "       %s [OPTION]... SOURCE DEST\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    struct stat dst_stat;
    int dst_is_dir = (stat(argv[argc-1], &dst_stat) == 0 && S_ISDIR(dst_stat.st_mode));

    if (argc - optind > 2 && !dst_is_dir) {
        fprintf(stderr, "mv: target '%s' is not a directory\n", argv[argc-1]);
        exit(EXIT_FAILURE);
    }

    for (int i = optind; i < argc - 1; i++) {
        char *dst_path;
        if (dst_is_dir) {
            char *filename = strrchr(argv[i], '/');
            filename = filename ? filename + 1 : argv[i];
            dst_path = malloc(strlen(argv[argc-1]) + strlen(filename) + 2);
            sprintf(dst_path, "%s/%s", argv[argc-1], filename);
        } else {
            dst_path = argv[argc-1];
        }

        if (rename(argv[i], dst_path) != 0) {
            if (!force) {
                fprintf(stderr, "mv: %s\n", strerror(errno));
                if (dst_is_dir) free(dst_path);
                exit(EXIT_FAILURE);
            }
        } else if (verbose) {
            printf("'%s' -> '%s'\n", argv[i], dst_path);
        }

        if (dst_is_dir) free(dst_path);
    }
    
    return EXIT_SUCCESS;
}
