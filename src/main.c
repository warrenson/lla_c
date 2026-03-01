#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "file.h"
#include "parse.h"

void print_usage(char *argv[]) {
    printf("Usage: %s [-n] -f <database_file>\n", argv[0]);
    printf("\t -n             - create new database file\n");
    printf("\t -f <filepath>  - (required) path to database file\n");
    printf("\t -l             - List employees\n");
    printf("\t -a <addstring> - Add employee from string: "
           "'<name>,<address>,<hours>'\n");
    return;
}

int main(int argc, char *argv[]) {
    char *filepath = NULL;
    char *author = NULL;
    char *addstring = NULL;
    bool newfile = false;
    bool list = false;
    int c;

    int dbfd = -1;
    struct dbheader_t *dbheader = NULL;
    struct employee_t *employees = NULL;

    while ((c = getopt(argc, argv, "lnf:a:")) != -1) {
        switch (c) {
        case 'l':
            list = true;
            break;
        case 'n':
            newfile = true;
            break;
        case 'f':
            filepath = optarg;
            break;
        case 'a':
            addstring = optarg;
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
        if (create_db_header(&dbheader) == STATUS_ERROR) {
            printf("Failed to create database header\n");
            return -1;
        }

    } else {
        dbfd = open_db_file(filepath);
        if (dbfd == STATUS_ERROR) {
            printf("Unable to open_db_file\n");
            return -1;
        }
        validate_db_header(dbfd, &dbheader);
    }

    if (read_employees(dbfd, dbheader, &employees) != STATUS_SUCCESS) {
        printf("Failed to read employees\n");
        return 0;
    }

    if (addstring) {
        add_employee(dbheader, &employees, addstring);
    }

    if (list) {
        list_employees(dbheader, employees);
    }

    // FINALLY (because it mangles the dbheader)
    output_file(dbfd, dbheader, employees);

    return 0;
}
