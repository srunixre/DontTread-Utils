#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc == 1) {
        char buffer[4096];
        ssize_t bytes;
        while ((bytes = read(0, buffer, sizeof(buffer))) > 0) {
            write(1, buffer, bytes);
        }
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        int fd = open(argv[i], O_RDONLY);
        if (fd == -1) {
            perror("cat");
            continue;
        }

        char buffer[4096];
        ssize_t bytes;
        while ((bytes = read(fd, buffer, sizeof(buffer))) > 0) {
            write(1, buffer, bytes);
        }

        close(fd);
    }
    return 0;
}
