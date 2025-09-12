#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <utime.h>
#include <getopt.h>

int main(int argc, char *argv[]) {
    int no_create = 0;
    int change_time = 0;
    
    int opt;
    while ((opt = getopt(argc, argv, "cr")) != -1) {
        switch (opt) {
            case 'c': no_create = 1; break;
            case 'r': change_time = 1; break;
            default: exit(EXIT_FAILURE);
        }
    }

    if (optind >= argc) {
        fprintf(stderr, "Usage: %s [OPTION]... FILE...\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (int i = optind; i < argc; i++) {
        if (no_create) {
            if (access(argv[i], F_OK) != 0) continue;
        }
        
        int fd = open(argv[i], O_WRONLY | O_CREAT, 0644);
        if (fd == -1) {
            fprintf(stderr, "touch: cannot touch '%s': %s\n", argv[i], strerror(errno));
            continue;
        }
        close(fd);

        if (utime(argv[i], NULL) != 0) {
            fprintf(stderr, "touch: cannot update time of '%s': %s\n", argv[i], strerror(errno));
        }
    }

    return EXIT_SUCCESS;
}
