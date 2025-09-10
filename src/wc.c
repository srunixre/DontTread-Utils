#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int fd = argc > 1 ? open(argv[1], O_RDONLY) : STDIN_FILENO;
    if (fd == -1) {
        perror("wc");
        return 1;
    }
    
    char buffer[1];
    int lines = 0, words = 0, chars = 0;
    int in_word = 0;
    
    while (read(fd, buffer, 1) > 0) {
        chars++;
        if (buffer[0] == '\n') lines++;
        if (buffer[0] == ' ' || buffer[0] == '\n' || buffer[0] == '\t') {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            words++;
        }
    }
    
    printf("%d %d %d\n", lines, words, chars);
    if (argc > 1) close(fd);
    return 0;
}
