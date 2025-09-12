#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <getopt.h>

int long_format = 0;
int all_files = 0;
int recursive = 0;

void print_long_format(const char *name, struct stat *st) {
    printf((S_ISDIR(st->st_mode)) ? "d" : "-");
    printf((st->st_mode & S_IRUSR) ? "r" : "-");
    printf((st->st_mode & S_IWUSR) ? "w" : "-");
    printf((st->st_mode & S_IXUSR) ? "x" : "-");
    printf((st->st_mode & S_IRGRP) ? "r" : "-");
    printf((st->st_mode & S_IWGRP) ? "w" : "-");
    printf((st->st_mode & S_IXGRP) ? "x" : "-");
    printf((st->st_mode & S_IROTH) ? "r" : "-");
    printf((st->st_mode & S_IWOTH) ? "w" : "-");
    printf((st->st_mode & S_IXOTH) ? "x" : "-");
    
    struct passwd *pw = getpwuid(st->st_uid);
    struct group *gr = getgrgid(st->st_gid);
    
    printf(" %2lu", st->st_nlink);
    printf(" %-8s", pw ? pw->pw_name : "unknown");
    printf(" %-8s", gr ? gr->gr_name : "unknown");
    printf(" %8ld", st->st_size);
    
    char time_buf[20];
    strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", localtime(&st->st_mtime));
    printf(" %s %s\n", time_buf, name);
}

void list_directory(const char *path, int show_total) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror("ls");
        return;
    }

    if (long_format && show_total) {
        long total = 0;
        struct dirent *entry;
        while ((entry = readdir(dir))) {
            if (!all_files && entry->d_name[0] == '.') continue;
            char full_path[1024];
            snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
            struct stat st;
            if (stat(full_path, &st) == 0) {
                total += st.st_blocks;
            }
        }
        printf("total %ld\n", total / 2);
        rewinddir(dir);
    }

    struct dirent *entry;
    while ((entry = readdir(dir))) {
        if (!all_files && entry->d_name[0] == '.') continue;
        
        if (long_format) {
            char full_path[1024];
            snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
            struct stat st;
            if (stat(full_path, &st) == 0) {
                print_long_format(entry->d_name, &st);
            }
        } else {
            printf("%s  ", entry->d_name);
        }
    }

    if (!long_format) printf("\n");
    closedir(dir);
}

int main(int argc, char *argv[]) {
    static struct option long_options[] = {
        {"all", no_argument, 0, 'a'},
        {"long", no_argument, 0, 'l'},
        {"recursive", no_argument, 0, 'R'},
        {0, 0, 0, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "alR", long_options, NULL)) != -1) {
        switch (opt) {
            case 'a': all_files = 1; break;
            case 'l': long_format = 1; break;
            case 'R': recursive = 1; break;
            default: exit(EXIT_FAILURE);
        }
    }

    if (optind >= argc) {
        list_directory(".", 1);
    } else {
        for (int i = optind; i < argc; i++) {
            if (argc - optind > 1) {
                printf("%s:\n", argv[i]);
            }
            list_directory(argv[i], 1);
            if (i < argc - 1) printf("\n");
        }
    }

    return EXIT_SUCCESS;
}
