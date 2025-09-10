#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int lines = 10;
    char *filename = NULL;
    
    if (argc > 1 && argv[1][0] == '-') {
        lines = atoi(argv[1] + 1);
        filename = argc > 2 ? argv[2] : NULL;
    } else {
        filename = argc > 1 ? argv[1] : NULL;
    }
    
    int fd = filename ? open(filename, O_RDONLY) : STDIN_FILENO;
    if (fd == -1) {
        perror("head");
        return 1;
    }
    
    char buffer[1];
    int count = 0;
    while (read(fd, buffer, 1) > 0) {
        write(STDOUT_FILENO, buffer, 1);
        if (buffer[0] == '\n') {
            count++;
            if (count >= lines) break;
        }
    }
    
    if (filename) close(fd);
    return 0;
}
