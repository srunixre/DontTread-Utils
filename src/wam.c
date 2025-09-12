#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>

int main() {
    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);
    if (pw) {
        printf("%s\n", pw->pw_name);
        return 0;
    } else {
        perror("whoami");
        return 1;
    }
}
