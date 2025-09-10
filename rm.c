#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

void remove_file(const char *path) {
    if (unlink(path) != 0) {
        perror("rm");
        exit(EXIT_FAILURE);
    }
}

void remove_directory(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror("rm");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir))) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        struct stat statbuf;
        if (stat(full_path, &statbuf) != 0) {
            perror("rm");
            closedir(dir);
            exit(EXIT_FAILURE);
        }

        if (S_ISDIR(statbuf.st_mode)) {
            remove_directory(full_path);
        } else {
            remove_file(full_path);
        }
    }

    closedir(dir);

    if (rmdir(path) != 0) {
        perror("rm");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [-d] [-f] <path>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int remove_dir = 0;
    char *target_path = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            remove_dir = 1;
        } else if (strcmp(argv[i], "-f") == 0) {
            continue;
        } else {
            target_path = argv[i];
        }
    }

    if (!target_path) {
        fprintf(stderr, "No target specified\n");
        return EXIT_FAILURE;
    }

    struct stat statbuf;
    if (stat(target_path, &statbuf) != 0) {
        perror("rm");
        return EXIT_FAILURE;
    }

    if (S_ISDIR(statbuf.st_mode)) {
        if (remove_dir) {
            remove_directory(target_path);
        } else {
            fprintf(stderr, "rm: %s is a directory (use -d)\n", target_path);
            return EXIT_FAILURE;
        }
    } else {
        remove_file(target_path);
    }

    return EXIT_SUCCESS;
}
