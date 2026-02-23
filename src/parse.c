#include <arpa/inet.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "common.h"
#include "parse.h"

// void list_employees(struct dbheader_t *dbhdr, struct employee_t *employees) {}

// int add_employee(struct dbheader_t *dbhdr, struct employee_t *employees,
                 // char *addstring) {}

// int read_employees(int fd, struct dbheader_t *dbhdr,
                   // struct employee_t **employeesOut) {}

int output_file(int fd, struct dbheader_t *dbhdr,
                struct employee_t *employees) {
    if (fd < 0) {
        printf("Got a bad FD from input\n");
        return STATUS_ERROR;
    }

    int realcount = dbhdr->count;

    dbhdr->magic = htonl(dbhdr->magic);
    dbhdr->filesize = htonl(sizeof(struct dbheader_t) +
                            (sizeof(struct employee_t) * realcount));
    dbhdr->count = htons(dbhdr->count);
    dbhdr->version = htons(dbhdr->version);

    lseek(fd, 0, SEEK_SET);
    write(fd, dbhdr, sizeof(struct dbheader_t));

    // int i = 0;
    // for (; i < realcount; i++) {
    // 	employees[i].hours = htonl(employees[i].hours);
    // 	write(fd, &employees[i], sizeof(struct employee_t));
    // }

    return STATUS_SUCCESS;
}

int validate_db_header(int fd, struct dbheader_t **headerOut) {
    if (fd < 0) {
        printf("Got a bad FD from input file.\n");
        return STATUS_ERROR;
    }

    struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
    if (header == NULL) {
        printf("Malloc failed to create a database header\n");
        return STATUS_ERROR;
    }
    if (read(fd, header, sizeof(struct dbheader_t)) !=
        sizeof(struct dbheader_t)) {
        printf("Failed to read database header from file.\n");
        perror("read");
        free(header);
        return STATUS_ERROR;
    }

    // decode data
    header->version = ntohs(header->version);
    header->count = ntohs(header->count);
    header->magic = ntohl(header->magic);
    header->filesize = ntohl(header->filesize);

    // test version
    if (header->version != 1) {
        printf("Incorrect database version %d, expected version 1",
               header->version);
        free(header);
        return STATUS_ERROR;
    }
    // test magic
    if (header->magic != HEADER_MAGIC) {
        printf("Incorrect database magic number %d", header->magic);
        free(header);
        return STATUS_ERROR;
    }
    // test filesize
    struct stat dbstat = {0};
    fstat(fd, &dbstat);
    if (header->filesize != dbstat.st_size) {
        printf("Corrupted database file.\n");
        free(header);
        return STATUS_ERROR;
    }

    // Assign output
    *headerOut = header;

    return STATUS_SUCCESS;
}

int create_db_header(int fd, struct dbheader_t **headerOut) {
    // make pointer to one header-size of memory
    struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
    if (header == NULL) {
        printf("Malloc failed to create DB header\n");
        return STATUS_ERROR;
    }

    // Set header values
    header->version = 0x1;
    header->count = 0;
    header->magic = HEADER_MAGIC;
    header->filesize = sizeof(struct dbheader_t);

    // Assign output
    *headerOut = header;

    return STATUS_SUCCESS;
}
