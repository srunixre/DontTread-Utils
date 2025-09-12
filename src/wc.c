#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>

int print_lines = 1;
int print_words = 1;
int print_chars = 1;

void count_file(const char *filename, int fd) {
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
    
    if (print_lines) printf("%8d", lines);
    if (print_words) printf("%8d", words);
    if (print_chars) printf("%8d", chars);
    if (filename) printf(" %s", filename);
    printf("\n");
}

int main(int argc, char *argv[]) {
    static struct option long_options[] = {
        {"lines", no_argument, 0, 'l'},
        {"words", no_argument, 0, 'w'},
        {"chars", no_argument, 0, 'c'},
        {0, 0, 0, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "lwcmLWC", long_options, NULL)) != -1) {
        switch (opt) {
            case 'l': case 'L': 
                print_lines = 1; print_words = 0; print_chars = 0; 
                break;
            case 'w': case 'W': 
                print_lines = 0; print_words = 1; print_chars = 0; 
                break;
            case 'c': case 'C': 
                print_lines = 0; print_words = 0; print_chars = 1; 
                break;
            case 'm': 
                print_lines = 0; print_words = 0; print_chars = 1;
                break;
            default: exit(EXIT_FAILURE);
        }
    }

    if (optind >= argc) {
        count_file(NULL, STDIN_FILENO);
        return 0;
    }

    long total_lines = 0, total_words = 0, total_chars = 0;
    
    for (int i = optind; i < argc; i++) {
        int fd = open(argv[i], O_RDONLY);
        if (fd == -1) {
            perror("wc");
            continue;
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
        
        if (print_lines) printf("%8d", lines);
        if (print_words) printf("%8d", words);
        if (print_chars) printf("%8d", chars);
        printf(" %s\n", argv[i]);
        
        total_lines += lines;
        total_words += words;
        total_chars += chars;
        close(fd);
    }

    if (argc - optind > 1) {
        if (print_lines) printf("%8ld", total_lines);
        if (print_words) printf("%8ld", total_words);
        if (print_chars) printf("%8ld", total_chars);
        printf(" total\n");
    }

    return 0;
}
