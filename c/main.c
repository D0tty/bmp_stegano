#define _XOPEN_SOURCE

#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "bmp.h"
#include "stegano.h"
#include "tool.h"

int main(int argc, char *argv[])
{
  int fdin = -1, fdout = -1, fddata = -1;
  struct stat st;
  int decode = 0;
  int opt;

  if(argc < 2){
    fprintf(stderr, "Usage: %s -i image -o output [-f data | -d] \n",
        argv[0]);
    exit(EXIT_FAILURE);
  }

  while ((opt = getopt(argc, argv, "i:o:f:d")) != -1){
    switch (opt) {
      case 'd':
        decode = 1;
        break;
      case 'i':
        fdin = open(optarg, O_RDWR);
        if(fdin == -1){
          printf("open image (in) failed\n");
          return 2;
        }
        break;
      case 'o':
        fdout = open(optarg, O_RDWR | O_CREAT, S_IRWXU);
        if(fdout == -1){
          printf("open/create file (out) failed");
          return 2;
        }
        break;
      case 'f':
        fddata = open(optarg, O_RDWR);
        if(fddata == -1){
          printf("open file (data) failed");
          return 2;
        }
        break;
      default:
        fprintf(stderr, "Usage: %s -i image -o output [-f data | -d] \n",
            argv[0]);
        exit(EXIT_FAILURE);
    }
  }

  if (fdin == -1 || fdout == -1){
    fprintf(stderr, "Usage: %s -i image -o output [-f data | -d] \n",
        argv[0]);
    exit(EXIT_FAILURE);
  }

  if(fstat(fdin, &st) == -1){
    printf("fstat failed");
    return 2;
  }
  size_t in_size = st.st_size;

  struct bmp_header *bh_in = get_bmp_header(fdin, in_size);
  if (bh_in == NULL){
    printf("mmap failed");
    return 2;
  }

  if (decode){
    stegano_unhide(get_image_data(bh_in), fdout);
  }
  else{
    struct bmp_header *bh_out = get_bmp_header(fdout, in_size);
    if (bh_out == NULL){
      printf("mmap failed");
      return 2;
    }

    if(!file_copy((char*)bh_in + 2, in_size, fdout)){
      return 3;
    }

    stegano_hide(get_image_data(bh_out), fddata);

    close(fddata);
    munmap(bh_out, in_size);
  }

  close(fdin);
  close(fdout);
  munmap(bh_in, in_size);
  return 0;
}
