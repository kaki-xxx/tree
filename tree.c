#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>
#include "flags.h"
#include "error.h"

static int compar(const void *a, const void *b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

static error isdir(const char *pathname, bool* res) {
    struct stat buf;
    if (lstat(pathname, &buf) < 0) {
        return fail(errno, pathname);
    }
    *res = S_ISDIR(buf.st_mode);
    return success();
}

static error listdir(DIR *dp, char *pathnames[], int *n, int all, int directory_only) {
    struct dirent *dirp;
    *n = 0;
    while (errno = 0, (dirp = readdir(dp)) != NULL) {
        if (!all && dirp->d_name[0] == '.') {
            continue;
        }
        if (!strcmp(dirp->d_name, ".") || !strcmp(dirp->d_name, "..")) {
            continue;
        }
        bool _isdir;
        error err = isdir(dirp->d_name, &_isdir);
        if (iserror(err)) {
            return err;
        }
        if (directory_only && !_isdir) {
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
        return fail(errno, "readdir");
    }
    qsort(pathnames, *n, sizeof(char*), compar);

    return success();
}

#define MAX_FILES 1024

/*
単純に左に横棒を入れてしまい
└── CompilerIdCXX
│   └── tmp
みたいになってしまうのを防ぐための変数
*/
static bool disp_line[MAX_FILES];

static error do_tree_internal(char *dirpath, int depth) {
    DIR *dp = opendir(dirpath);
    if (dp == NULL) {
        return fail(errno, dirpath);
    }

    #define PATH_LEN 1024
    char prevcwd[PATH_LEN];

    if (getcwd(prevcwd, PATH_LEN) == NULL) {
        return fail(errno, dirpath);
    }

    if (chdir(dirpath) < 0) {
        return fail(errno, dirpath);
    }

    printf("%s\n", dirpath);

    char *file_names[MAX_FILES];
    int n = MAX_FILES;
    error err = listdir(dp, file_names, &n, flags.all, flags.directory_only);
    if (iserror(err)) {
        return err;
    }

    char *line = "├";
    for (int i = 0; i < n; i++) {
        disp_line[depth + 1] = true;
        if (i == n - 1) {
            line = "└";
            disp_line[depth + 1] = false;
        }
        int d = 0;
        while (d++ < depth) {
            if (disp_line[d]) printf("│   ");
            else printf("    ");
        } 
        bool _isdir;
        error err = isdir(file_names[i], &_isdir);
        if (iserror(err)) {
            return err;
        }
        if (_isdir) {
            printf("%s── ", line);
            error err = do_tree_internal(file_names[i], depth + 1);
            if (iserror(err)) {
                return err;
            }
        } else {
            printf("%s── %s\n", line, file_names[i]);
        }
    }

    if (closedir(dp) < 0) {
        return fail(errno, dirpath);
    }

    if (chdir(prevcwd)) {
        return fail(errno, prevcwd);
    }
    return success();
}

error do_tree(char *dirpath) {
    return do_tree_internal(dirpath, 0);
}
