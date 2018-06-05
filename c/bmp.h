#ifndef BMP_H
#define BMP_H

#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

struct bmp_img_head {
  unsigned int          header_size;
  unsigned int          width;
  unsigned int          height;
  short int             planes;
  short int             bit;
  unsigned int          comp_method;
  unsigned int          image_size;
  int                   hres;
  int                   vres;
  unsigned int          color_number;
  unsigned int          important_color;
};

struct bmp_header {
  short                 padding;   // trick to align header
  char                  magic[2];
  unsigned int          file_size;
  unsigned int          reserved;
  unsigned int          offset;
  struct bmp_img_head   dib;
  unsigned char         data[4];   // entry point for data
};

/*
static
char max_char(unsigned long val)
{
  return (val >= 255) ? 255 : val;
}
*/

struct bmp_header *get_bmp_header(int fd, size_t file_size);

void print_header(struct bmp_header *h);

char *get_image_data(struct bmp_header *bh);

size_t get_image_size(struct bmp_header *bh);

/*
void make_redish(unsigned char *image_data, size_t data_length);
*/

#endif
