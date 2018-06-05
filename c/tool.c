#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int file_copy(char *in, size_t len, int out){
  size_t bytes = write(out, in, len);
  if (bytes != len){
    perror(NULL);
  }

  return bytes == len;
}
