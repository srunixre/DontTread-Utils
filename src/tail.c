#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <getopt.h>

int main(int argc, char *argv[]) {
    int lines = 10;
    int quiet = 0;
    char *filename = NULL;
    
    static struct option long_options[] = {
        {"lines", required_argument, 0, 'n'},
        {"quiet", no_argument, 0, 'q'},
        {0, 0, 0, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "n:q", long_options, NULL)) != -1) {
        switch (opt) {
            case 'n': lines = atoi(optarg); break;
            case 'q': quiet = 1; break;
            default: exit(EXIT_FAILURE);
        }
    }

    if (optind < argc) {
        filename = argv[optind];
    }

    int fd = filename ? open(filename, O_RDONLY) : STDIN_FILENO;
    if (fd == -1) {
        perror("tail");
        return 1;
    }

    if (!quiet && filename && argc - optind > 1) {
        printf("==> %s <==\n", filename);
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

    if (filename) {
        close(fd);
        if (optind < argc - 1) {
            printf("\n");
        }
    }

    return 0;
}
