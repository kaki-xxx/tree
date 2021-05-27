#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>

int compar(const void *a, const void *b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

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

    #define MAX_FILES 1024
    char *file_names[MAX_FILES];

    struct dirent *dirp;
    int n = 0;
    while (errno = 0, (dirp = readdir(dp)) != NULL) {
        if (dirp->d_name[0] == '.') {
            continue;
        }
        size_t len = strlen(dirp->d_name);
        char *file_name = malloc((len + 1) * sizeof(char));
        if (file_name == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        strcpy(file_name, dirp->d_name);
        file_names[n] = file_name;
        n++;
    }
    if (errno != 0) {
        perror(argv[1]);
        exit(EXIT_FAILURE);
    }
    qsort(file_names, n, sizeof(char*), compar);
    for (int i = 0; i < n; i++) {
        printf("%s\n", file_names[i]);
    }

    if (closedir(dp) < 0) {
        perror(argv[1]);
        exit(EXIT_FAILURE);
    }

    return 0;
}
