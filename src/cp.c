#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <dirent.h>

int recursive = 0;
int verbose = 0;
int force = 0;

void copy_file(const char *src, const char *dst) {
    struct stat src_stat;
    if (stat(src, &src_stat) != 0) {
        if (!force) {
            fprintf(stderr, "cp: cannot stat '%s': %s\n", src, strerror(errno));
            exit(EXIT_FAILURE);
        }
        return;
    }

    if (S_ISDIR(src_stat.st_mode)) {
        if (!recursive) {
            fprintf(stderr, "cp: -r not specified; omitting directory '%s'\n", src);
            return;
        }
        
        mkdir(dst, src_stat.st_mode);
        
        DIR *dir = opendir(src);
        if (!dir) {
            if (!force) {
                fprintf(stderr, "cp: cannot open directory '%s': %s\n", src, strerror(errno));
                exit(EXIT_FAILURE);
            }
            return;
        }

        struct dirent *entry;
        while ((entry = readdir(dir))) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
            
            char src_path[1024], dst_path[1024];
            snprintf(src_path, sizeof(src_path), "%s/%s", src, entry->d_name);
            snprintf(dst_path, sizeof(dst_path), "%s/%s", dst, entry->d_name);
            copy_file(src_path, dst_path);
        }
        closedir(dir);
    } else {
        int src_fd = open(src, O_RDONLY);
        if (src_fd == -1) {
            if (!force) {
                fprintf(stderr, "cp: cannot open '%s': %s\n", src, strerror(errno));
                exit(EXIT_FAILURE);
            }
            return;
        }

        int dst_fd = open(dst, O_WRONLY | O_CREAT | O_TRUNC, src_stat.st_mode);
        if (dst_fd == -1) {
            if (!force) {
                fprintf(stderr, "cp: cannot create '%s': %s\n", dst, strerror(errno));
                close(src_fd);
                exit(EXIT_FAILURE);
            }
            close(src_fd);
            return;
        }

        char buffer[4096];
        ssize_t bytes_read;
        while ((bytes_read = read(src_fd, buffer, sizeof(buffer))) > 0) {
            if (write(dst_fd, buffer, bytes_read) != bytes_read) {
                if (!force) {
                    fprintf(stderr, "cp: error writing to '%s': %s\n", dst, strerror(errno));
                    close(src_fd);
                    close(dst_fd);
                    exit(EXIT_FAILURE);
                }
                break;
            }
        }

        close(src_fd);
        close(dst_fd);
    }

    if (verbose) {
        printf("'%s' -> '%s'\n", src, dst);
    }
}

int main(int argc, char *argv[]) {
    static struct option long_options[] = {
        {"recursive", no_argument, 0, 'r'},
        {"verbose", no_argument, 0, 'v'},
        {"force", no_argument, 0, 'f'},
        {0, 0, 0, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "rvf", long_options, NULL)) != -1) {
        switch (opt) {
            case 'r': recursive = 1; break;
            case 'v': verbose = 1; break;
            case 'f': force = 1; break;
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
        fprintf(stderr, "cp: target '%s' is not a directory\n", argv[argc-1]);
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

        copy_file(argv[i], dst_path);

        if (dst_is_dir) free(dst_path);
    }

    return EXIT_SUCCESS;
}
