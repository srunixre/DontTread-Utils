#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    struct stat st;
    if (stat(argv[2], &st) == 0 && S_ISDIR(st.st_mode)) {
        char *filename = strrchr(argv[1], '/');
        if (filename == NULL) filename = argv[1];
        else filename++;
        
        char new_path[1024];
        snprintf(new_path, sizeof(new_path), "%s/%s", argv[2], filename);
        
        if (rename(argv[1], new_path) != 0) {
            fprintf(stderr, "mv: %s\n", strerror(errno));
            return EXIT_FAILURE;
        }
    } else {
        if (rename(argv[1], argv[2]) != 0) {
            fprintf(stderr, "mv: %s\n", strerror(errno));
            return EXIT_FAILURE;
        }
    }
    
    return EXIT_SUCCESS;
}
