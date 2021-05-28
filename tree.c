#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>

static int compar(const void *a, const void *b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

int do_tree(char *dirpath) {
    DIR *dp = opendir(dirpath);
    if (dp == NULL) {
        return -1;
    }

    printf("%s\n", dirpath);

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
        return -1;
    }
    qsort(file_names, n, sizeof(char*), compar);
    for (int i = 0; i < n - 1; i++) {
        printf("├── %s\n", file_names[i]);
    }
    printf("└── %s\n", file_names[n - 1]);

    if (closedir(dp) < 0) {
        return -1;
    }

    return 0;
}
