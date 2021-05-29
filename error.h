#ifndef ERROR_H
#define ERROR_H

#include <stdbool.h>

typedef struct {
    int _errno;
    const char *context;
} error;

error success();
error fail(int _errno, const char* context);
bool iserror(error err);
void err_exit(error err);

#endif /* ERROR_H */
