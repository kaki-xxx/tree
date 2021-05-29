#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>
#include "flags.h"

static int compar(const void *a, const void *b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

static int isdir(const char *pathname) {
    struct stat buf;
    if (lstat(pathname, &buf) < 0) {
        return -1;
    }
    return S_ISDIR(buf.st_mode);
}

static int listdir(DIR *dp, char *pathnames[], int *n, int all, int directory_only) {
    struct dirent *dirp;
    *n = 0;
    while (errno = 0, (dirp = readdir(dp)) != NULL) {
        if (!all && dirp->d_name[0] == '.') {
            continue;
        }
        if (!strcmp(dirp->d_name, ".") || !strcmp(dirp->d_name, "..")) {
            continue;
        }
        if (directory_only && !isdir(dirp->d_name)) {
            continue;
        }
        size_t len = strlen(dirp->d_name);
        char *file_name = malloc((len + 1) * sizeof(char));
        if (file_name == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        strcpy(file_name, dirp->d_name);
        pathnames[*n] = file_name;
        (*n)++;
    }
    if (errno != 0) {
        return -1;
    }
    qsort(pathnames, *n, sizeof(char*), compar);

    return 0;
}

extern struct flags flags;

#define MAX_FILES 1024

/*
単純に左に横棒を入れてしまい
└── CompilerIdCXX
│   └── tmp
みたいになってしまうのを防ぐための変数
*/
static bool displine[MAX_FILES];

static int do_tree_internal(char *dirpath, int depth) {
    DIR *dp = opendir(dirpath);
    if (dp == NULL) {
        return -1;
    }

    chdir(dirpath);

    printf("%s\n", dirpath);

    char *file_names[MAX_FILES];
    int n = MAX_FILES;
    listdir(dp, file_names, &n, flags.all, flags.directory_only);

    char *line = "├";
    for (int i = 0; i < n; i++) {
        displine[depth + 1] = true;
        if (i == n - 1) {
            line = "└";
            displine[depth + 1] = false;
        }
        int d = 0;
        while (d++ < depth) {
            if (displine[d]) printf("│   ");
            else printf("    ");
        } 
        int dir;
        if ((dir = isdir(file_names[i])) < 0) {
            return -1;
        } else if (dir) {
            printf("%s── ", line);
            do_tree_internal(file_names[i], depth + 1);
        } else {
            printf("%s── %s\n", line, file_names[i]);
        }
    }

    if (closedir(dp) < 0) {
        return -1;
    }

    chdir("..");
    return 0;
}

int do_tree(char *dirpath) {
    return do_tree_internal(dirpath, 0);
}
