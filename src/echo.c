#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

int main(int argc, char *argv[]) {
    int no_newline = 0;
    int enable_backslash = 0;
    int i = 1;

    while (i < argc && argv[i][0] == '-') {
        if (strcmp(argv[i], "-n") == 0) {
            no_newline = 1;
            i++;
        } else if (strcmp(argv[i], "-e") == 0) {
            enable_backslash = 1;
            i++;
        } else if (strcmp(argv[i], "-E") == 0) {
            enable_backslash = 0;
            i++;
        } else if (strcmp(argv[i], "--help") == 0) {
            printf("Usage: echo [SHORT-OPTION]... [STRING]...\n");
            printf("  or:  echo LONG-OPTION\n");
            printf("Echo the STRING(s) to standard output.\n\n");
            printf("  -n             do not output the trailing newline\n");
            printf("  -e             enable interpretation of backslash escapes\n");
            printf("  -E             disable interpretation of backslash escapes (default)\n");
            printf("      --help     display this help and exit\n");
            printf("      --version  output version information and exit\n\n");
            return EXIT_SUCCESS;
        } else if (strcmp(argv[i], "--version") == 0) {
            printf("echo (DontTread Utils) 1.0\n");
            return EXIT_SUCCESS;
        } else {
            break;
        }
    }

    for (; i < argc; i++) {
        char *str = argv[i];
        if (enable_backslash) {
            for (int j = 0; str[j] != '\0'; j++) {
                if (str[j] == '\\') {
                    j++;
                    switch (str[j]) {
                        case 'a': putchar('\a'); break;
                        case 'b': putchar('\b'); break;
                        case 'c': return EXIT_SUCCESS;
                        case 'e': putchar('\033'); break;
                        case 'f': putchar('\f'); break;
                        case 'n': putchar('\n'); break;
                        case 'r': putchar('\r'); break;
                        case 't': putchar('\t'); break;
                        case 'v': putchar('\v'); break;
                        case '\\': putchar('\\'); break;
                        case '0': {
                            int oct = 0;
                            int digits = 0;
                            while (str[j+1] >= '0' && str[j+1] <= '7' && digits < 3) {
                                j++;
                                oct = oct * 8 + (str[j] - '0');
                                digits++;
                            }
                            putchar(oct);
                            break;
                        }
                        case 'x': {
                            int hex = 0;
                            int digits = 0;
                            while (digits < 2) {
                                j++;
                                if (str[j] >= '0' && str[j] <= '9') {
                                    hex = hex * 16 + (str[j] - '0');
                                } else if (str[j] >= 'a' && str[j] <= 'f') {
                                    hex = hex * 16 + (str[j] - 'a' + 10);
                                } else if (str[j] >= 'A' && str[j] <= 'F') {
                                    hex = hex * 16 + (str[j] - 'A' + 10);
                                } else {
                                    j--;
                                    break;
                                }
                                digits++;
                            }
                            putchar(hex);
                            break;
                        }
                        default:
                            putchar('\\');
                            putchar(str[j]);
                            break;
                    }
                } else {
                    putchar(str[j]);
                }
            }
        } else {
            printf("%s", str);
        }
        
        if (i < argc - 1) {
            putchar(' ');
        }
    }
    
    if (!no_newline) {
        putchar('\n');
    }
    
    return EXIT_SUCCESS;
}
