#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <getopt.h>
#include <unistd.h>
#include <stdbool.h>
#include "tree.h"
#include "config.h"

int help = 0;
int version = 0;

void show_usage(const char *progname) {
    printf("Usage: %s [OPTION]... DIR\n", progname);
    printf("list contents of directories in a tree-like format\n");
    printf("\n");
    printf("     --help     print this message and exit\n");
    printf("     --version  output version infomation and exit\n");
}

void show_version(void) {
    printf("tree %d.%d\n", TREE_VERSION_MAJOR, TREE_VERSION_MINOR);
}

int main(int argc, char *argv[]) {
    struct option longopts[] = {
        { "help",    no_argument, &help,    1 },
        { "version", no_argument, &version, 1 },
    };

    if (argc != 2) {
        fprintf(stderr, "Usage: %s dir\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char opt;
    int longindex;
    while ((opt = getopt_long(argc, argv, "", longopts, &longindex)) != -1) {}

    if (help) {
        show_usage(argv[0]);
        exit(EXIT_SUCCESS);
    }

    if (version) {
        show_version();
        exit(EXIT_SUCCESS);
    }

    if (do_tree(argv[1]) < 0) {
        perror(argv[1]);
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
