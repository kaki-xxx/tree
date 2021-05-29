#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <stdbool.h>
#include "error.h"
#include "tree.h"
#include "config.h"
#include "flags.h"

struct flags flags;

void show_usage(const char *progname) {
    printf("Usage: %s [OPTION]... DIR...\n", progname);
    printf("list contents of directories in a tree-like format\n");
    printf("\n");
    printf(" -a, --all      do not ignore entries starting with .\n");
    printf(" -d             list directories only\n");
    printf("     --help     print this message and exit\n");
    printf("     --version  output version infomation and exit\n");
}

void show_version(void) {
    printf("tree %d.%d\n", TREE_VERSION_MAJOR, TREE_VERSION_MINOR);
}

int main(int argc, char *argv[]) {
    struct option longopts[] = {
        { "all",     no_argument, NULL,           'a' },
        { "help",    no_argument, &flags.help,    1 },
        { "version", no_argument, &flags.version, 1 },
    };

    char opt;
    int longindex;
    opterr = 1;
    while ((opt = getopt_long(argc, argv, "ad", longopts, &longindex)) != -1) {
        switch (opt) {
            case 'a':
                flags.all = 1;
                break;
            case 'd':
                flags.directory_only = 1;
                break;
            case 0:
                continue;
            default:
                exit(EXIT_FAILURE);
        }
    }

    if (flags.help) {
        show_usage(argv[0]);
        exit(EXIT_SUCCESS);
    }

    if (flags.version) {
        show_version();
        exit(EXIT_SUCCESS);
    }

    for (int i = optind; i < argc; i++) {
        error err = do_tree(argv[i]);
        if (iserror(err)) {
            err_exit(err);
        }
    }

    return EXIT_SUCCESS;
}
