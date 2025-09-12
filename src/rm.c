#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <getopt.h>

int recursive = 0;
int force = 0;
int verbose = 0;

void remove_file(const char *path) {
    if (unlink(path) != 0) {
        if (!force) {
            perror("rm");
            exit(EXIT_FAILURE);
        }
    } else if (verbose) {
        printf("removed '%s'\n", path);
    }
}

void remove_directory(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) {
        if (!force) {
            perror("rm");
            exit(EXIT_FAILURE);
        }
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir))) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        struct stat statbuf;
        if (stat(full_path, &statbuf) != 0) {
            if (!force) {
                perror("rm");
                closedir(dir);
                exit(EXIT_FAILURE);
            }
            continue;
        }

        if (S_ISDIR(statbuf.st_mode)) {
            remove_directory(full_path);
        } else {
            remove_file(full_path);
        }
    }

    closedir(dir);

    if (rmdir(path) != 0) {
        if (!force) {
            perror("rm");
            exit(EXIT_FAILURE);
        }
    } else if (verbose) {
        printf("removed directory '%s'\n", path);
    }
}

int main(int argc, char *argv[]) {
    static struct option long_options[] = {
        {"recursive", no_argument, 0, 'r'},
        {"force", no_argument, 0, 'f'},
        {"verbose", no_argument, 0, 'v'},
        {0, 0, 0, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "rfv", long_options, NULL)) != -1) {
        switch (opt) {
            case 'r': recursive = 1; break;
            case 'f': force = 1; break;
            case 'v': verbose = 1; break;
            default: exit(EXIT_FAILURE);
        }
    }

    if (optind >= argc) {
        fprintf(stderr, "Usage: %s [OPTION]... FILE...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    for (int i = optind; i < argc; i++) {
        struct stat statbuf;
        if (stat(argv[i], &statbuf) != 0) {
            if (!force) {
                perror("rm");
                exit(EXIT_FAILURE);
            }
            continue;
        }

        if (S_ISDIR(statbuf.st_mode)) {
            if (recursive) {
                remove_directory(argv[i]);
            } else {
                fprintf(stderr, "rm: cannot remove '%s': Is a directory\n", argv[i]);
                if (!force) exit(EXIT_FAILURE);
            }
        } else {
            remove_file(argv[i]);
        }
    }

    return EXIT_SUCCESS;
}
