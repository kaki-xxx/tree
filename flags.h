#ifndef FLAGS_H
#define FLAGS_H

struct flags {
    int help;
    int version;
    int all;
    int directory_only;
};

extern struct flags flags;

#endif /* FLAGS_H */
