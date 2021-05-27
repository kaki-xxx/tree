#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include "tree.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s dir\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (do_tree(argv[1]) < 0) {
        perror(argv[1]);
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
