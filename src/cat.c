#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>

int main(int argc, char *argv[]) {
    int show_ends = 0;
    int number = 0;
    int squeeze = 0;
    int show_tabs = 0;

    int opt;
    while ((opt = getopt(argc, argv, "EnsT")) != -1) {
        switch (opt) {
            case 'E': show_ends = 1; break;
            case 'n': number = 1; break;
            case 's': squeeze = 1; break;
            case 'T': show_tabs = 1; break;
            default: exit(EXIT_FAILURE);
        }
    }

    if (optind >= argc) {
        char buffer[4096];
        ssize_t bytes;
        int line_num = 1;
        int last_empty = 0;
        
        while ((bytes = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
            for (int i = 0; i < bytes; i++) {
                if (number && (i == 0 || buffer[i-1] == '\n')) {
                    printf("%6d\t", line_num++);
                }
                
                if (squeeze && buffer[i] == '\n') {
                    if (last_empty) continue;
                    last_empty = 1;
                } else {
                    last_empty = 0;
                }
                
                if (show_ends && buffer[i] == '\n') {
                    printf("$\n");
                } else if (show_tabs && buffer[i] == '\t') {
                    printf("^I");
                } else {
                    putchar(buffer[i]);
                }
            }
        }
        return EXIT_SUCCESS;
    }

    for (int i = optind; i < argc; i++) {
        int fd = open(argv[i], O_RDONLY);
        if (fd == -1) {
            perror("cat");
            continue;
        }

        char buffer[4096];
        ssize_t bytes;
        int line_num = 1;
        int last_empty = 0;
        
        while ((bytes = read(fd, buffer, sizeof(buffer))) > 0) {
            for (int i = 0; i < bytes; i++) {
                if (number && (i == 0 || buffer[i-1] == '\n')) {
                    printf("%6d\t", line_num++);
                }
                
                if (squeeze && buffer[i] == '\n') {
                    if (last_empty) continue;
                    last_empty = 1;
                } else {
                    last_empty = 0;
                }
                
                if (show_ends && buffer[i] == '\n') {
                    printf("$\n");
                } else if (show_tabs && buffer[i] == '\t') {
                    printf("^I");
                } else {
                    putchar(buffer[i]);
                }
            }
        }

        close(fd);
    }
    
    return EXIT_SUCCESS;
}
