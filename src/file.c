#include <stdio.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "common.h"
#include "file.h"

int create_db_file(char *filename) {
  int fd = open(filename, O_RDWR | O_CREAT, 06444);
  if (fd == -1) {
    perror("open");
    return STATUS_ERROR;
  }

  return 0;
}

int open_db_file(char *filename) { return 0; }
