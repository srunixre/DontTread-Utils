#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

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

int main(int argc, char *argv[]) {
    int long_format = 0;
    int all_files = 0;
    char *target = ".";
    
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (strchr(argv[i], 'l')) long_format = 1;
            if (strchr(argv[i], 'a')) all_files = 1;
        } else {
            target = argv[i];
        }
    }
    
    DIR *dir = opendir(target);
    if (!dir) {
        perror("ls");
        return EXIT_FAILURE;
    }
    
    struct dirent *entry;
    while ((entry = readdir(dir))) {
        if (!all_files && entry->d_name[0] == '.') continue;
        
        if (long_format) {
            char full_path[1024];
            snprintf(full_path, sizeof(full_path), "%s/%s", target, entry->d_name);
            
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
    return EXIT_SUCCESS;
}
