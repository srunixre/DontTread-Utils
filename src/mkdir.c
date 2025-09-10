#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "mkdir: missing operand\n");
        return EXIT_FAILURE;
    }

    int parents = 0;
    int mode = 0755;
    int start_index = 1;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--parents") == 0) {
                parents = 1;
                start_index++;
            } else if (strcmp(argv[i], "-m") == 0) {
                if (i + 1 >= argc) {
                    fprintf(stderr, "mkdir: missing operand to '-m'\n");
                    return EXIT_FAILURE;
                }
                mode = strtol(argv[i + 1], NULL, 8);
                start_index += 2;
                i++;
            } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
                start_index++;
            } else {
                fprintf(stderr, "mkdir: invalid option '%s'\n", argv[i]);
                return EXIT_FAILURE;
            }
        } else {
            break;
        }
    }

    if (start_index >= argc) {
        fprintf(stderr, "mkdir: missing operand\n");
        return EXIT_FAILURE;
    }

    for (int i = start_index; i < argc; i++) {
        if (parents) {
            char *path = argv[i];
            char *slash = path;
            
            while (*slash == '/') slash++;
            
            while ((slash = strchr(slash, '/')) != NULL) {
                *slash = '\0';
                if (mkdir(path, mode) != 0 && errno != EEXIST) {
                    perror("mkdir");
                    return EXIT_FAILURE;
                }
                *slash = '/';
                slash++;
                while (*slash == '/') slash++;
            }
        }
        
        if (mkdir(argv[i], mode) != 0) {
            if (errno == EEXIST && parents) {
                continue;
            }
            perror("mkdir");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
