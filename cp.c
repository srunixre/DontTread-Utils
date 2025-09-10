#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int src_fd = open(argv[1], O_RDONLY);
    if (src_fd == -1) {
        fprintf(stderr, "cp: cannot stat '%s': %s\n", argv[1], strerror(errno));
        return EXIT_FAILURE;
    }

    struct stat src_stat;
    if (fstat(src_fd, &src_stat) != 0) {
        fprintf(stderr, "cp: cannot stat '%s': %s\n", argv[1], strerror(errno));
        close(src_fd);
        return EXIT_FAILURE;
    }

    struct stat dst_stat;
    char dst_path[1024];
    
    if (stat(argv[2], &dst_stat) == 0 && S_ISDIR(dst_stat.st_mode)) {
        char *filename = strrchr(argv[1], '/');
        if (filename == NULL) {
            filename = argv[1];
        } else {
            filename++;
        }
        snprintf(dst_path, sizeof(dst_path), "%s/%s", argv[2], filename);
    } else {
        strncpy(dst_path, argv[2], sizeof(dst_path));
    }

    int dst_fd = open(dst_path, O_WRONLY | O_CREAT | O_TRUNC, src_stat.st_mode);
    if (dst_fd == -1) {
        fprintf(stderr, "cp: cannot create '%s': %s\n", dst_path, strerror(errno));
        close(src_fd);
        return EXIT_FAILURE;
    }

    char buffer[4096];
    ssize_t bytes_read;
    while ((bytes_read = read(src_fd, buffer, sizeof(buffer))) > 0) {
        ssize_t bytes_written = write(dst_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            fprintf(stderr, "cp: error writing to '%s': %s\n", dst_path, strerror(errno));
            close(src_fd);
            close(dst_fd);
            return EXIT_FAILURE;
        }
    }

    if (bytes_read == -1) {
        fprintf(stderr, "cp: error reading from '%s': %s\n", argv[1], strerror(errno));
        close(src_fd);
        close(dst_fd);
        return EXIT_FAILURE;
    }

    close(src_fd);
    close(dst_fd);
    return EXIT_SUCCESS;
}
