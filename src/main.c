#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "file.h"
#include "parse.h"

void print_usage(char *argv[]) {
    printf("Usage: %s [-n] -f <database_file>\n", argv[0]);
    printf("\t -n            - create new database file\n");
    printf("\t -f <filepath> - (required) path to database file\n");
    return;
}

int main(int argc, char *argv[]) {
    char *filepath = NULL;
    char *author = NULL;
    bool newfile = false;
    int c;

    int dbfd = -1;

    while ((c = getopt(argc, argv, "nf:")) != -1) {
        switch (c) {
        case 'n':
            newfile = true;
            break;
        case 'f':
            filepath = optarg;
            break;
        case '?':
            // printf("Unknown option -%c\n", c);
            print_usage(argv);
            return 0;
        default:
            return -1;
        }
    }

    if (filepath == NULL) {
        printf("Filepath argument is required!\n");
        print_usage(argv);
        return 0;
    }

    if (newfile) {
        dbfd = create_db_file(filepath);
        if (dbfd == STATUS_ERROR) {
            printf("Unable to create_db_file\n");
            return -1;
        }
    } else {
        dbfd = open_db_file(filepath);
        if (dbfd == STATUS_ERROR) {
            printf("Unable to open_db_file\n");
            return -1;
        }
    }

    printf("Newfile: %d\n", newfile);
    printf("Filepath: %s\n", filepath);

    return 0;
}
