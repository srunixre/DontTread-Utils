#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

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
        perror("tail");
        return 1;
    }
    
    off_t size = lseek(fd, 0, SEEK_END);
    off_t pos = size;
    int count = 0;
    char buffer[1];
    
    while (pos > 0 && count <= lines) {
        pos--;
        lseek(fd, pos, SEEK_SET);
        read(fd, buffer, 1);
        if (buffer[0] == '\n') count++;
    }
    
    lseek(fd, pos + (count > lines ? 1 : 0), SEEK_SET);
    while (read(fd, buffer, 1) > 0) {
        write(STDOUT_FILENO, buffer, 1);
    }
    
    if (filename) close(fd);
    return 0;
}
