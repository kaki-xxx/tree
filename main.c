#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s dir\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    DIR *dp = opendir(argv[1]);
    if (dp == NULL) {
        perror(argv[1]);
        exit(EXIT_FAILURE);
    }

    struct dirent* dirp;
    while (errno = 0, (dirp = readdir(dp)) != NULL) {
        printf("%s\n", dirp->d_name);
    }
    if (errno != 0) {
        perror(argv[1]);
        exit(EXIT_FAILURE);
    }

    if (closedir(dp) < 0) {
        perror(argv[1]);
        exit(EXIT_FAILURE);
    }

    return 0;
}
