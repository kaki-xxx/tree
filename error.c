#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "error.h"

error success() {
    error ret = { 0, NULL };
    return ret;
}

error fail(int _errno, const char* context) {
    char *buf = malloc(strlen(context) + 1);
    strcpy(buf, context);
    error ret = { _errno, buf };
    return ret;
}

bool iserror(error err) {
    return err._errno != 0;
}

void err_exit(error err) {
    fprintf(stderr, "%s: %s\n", err.context, strerror(err._errno));
    exit(EXIT_FAILURE);
}
