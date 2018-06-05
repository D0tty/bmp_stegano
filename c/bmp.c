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

struct bmp_header *get_bmp_header(int fd, size_t file_size){
  void *p = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if(p == MAP_FAILED)
  {
    return NULL;
  }

  return (struct bmp_header*)((char*)p - 2);
}

void print_header(struct bmp_header *h){
  char *fmt_str =
    "magic: \t\t\033[1m\033[32m%c%c\033[0m\n"
    "file_size: \t%d\n"
    "offset: \t%d\n"
    "width: \t\t\033[1m\033[31m%d\033[0m\n"
    "height: \t\033[1m\033[31m%d\033[0m\n"
    "bit: \t\t%d\n"
    "image_size: \t%d\n"
    "hres: \t\t%d\n"
    "vres: \t\t%d\n"
    "color_nb: \t%d\n";
  printf(fmt_str,
      *h->magic,
      *(h->magic+1),
      h->file_size,
      h->offset,
      h->dib.width,
      h->dib.height,
      h->dib.bit,
      h->dib.image_size,
      h->dib.hres,
      h->dib.vres,
      h->dib.color_number);
}

char *get_image_data(struct bmp_header *bh){
 return  (char*)bh + bh->offset;
}

size_t get_image_size(struct bmp_header *bh){
  return bh->dib.image_size; // size in bytes
}

/*
void make_redish(unsigned char *image_data, size_t data_length){
  unsigned char *g = image_data;
  unsigned char *r = image_data+1;
  unsigned char *b = image_data+2;

  for (size_t i = 0; i < data_length; i += 3)
  {
    float a = 0.3;      // alpha for trensparency
    float oa = 1 - a;   // one minus alpha

    r[i] = max_char(a*255 + oa * r[i]);
    g[i] = max_char(a*0 + oa * g[i]);
    b[i] = max_char(a*0 + oa * b[i]);
  }
}
*/
