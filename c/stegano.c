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

#include "stegano.h"

void set_int_at(char *image_data, int *offset, int n){
  image_data += *offset * 3;
  unsigned char *p1 = (unsigned char*)image_data;
  unsigned char *p2 = (unsigned char*)image_data+1;
  unsigned char *p3 = (unsigned char*)image_data+2;

  size_t nb_bits = 8 * sizeof(int);
  size_t ct_bits = 0;
  int nb_pix = (nb_bits/3) + (nb_bits%3 != 0);
  for (int i = 0; i < nb_pix; ++i)
  {
    int idx = i * 3;
    if (ct_bits++ < nb_bits){
      p1[idx] = (p1[idx] - (p1[idx] % 2)) + (n%2);
      n = n >> 1;
    }
    if (ct_bits++ < nb_bits){
      p2[idx] = (p2[idx] - (p2[idx] % 2)) + (n%2);
      n = n >> 1;
    }
    if (ct_bits++ < nb_bits){
      p3[idx] = (p3[idx] - (p3[idx] % 2)) + (n%2);
      n = n >> 1;
    }
  }
  *offset += nb_pix;
}

int get_int_at(char *image_data, int *offset){
  image_data += *offset * 3;
  char *p1 = image_data;
  char *p2 = image_data+1;
  char *p3 = image_data+2;

  int val = 0;
  size_t  nb_bits = 8 * sizeof(int);
  size_t  ct_bits = 0;
  int nb_pix = (nb_bits / 3) + (nb_bits%3 != 0);
  for (int i = 0; i < nb_pix; ++i)
  {
    int idx = i * 3;
    if (ct_bits < nb_bits){
      val += (1 << ct_bits++) * ((p1[idx])%2);
    }
    if (nb_bits - ct_bits != 0){
      val += (1 << ct_bits++) * ((p2[idx])%2);
    }
    if (nb_bits - ct_bits != 0){
      val += (1 << ct_bits++) * ((p3[idx])%2);
    }
  }
  *offset += nb_pix;

  return val;
}

void set_char_at(char *image_data, int *offset, char n){
  image_data += *offset * 3;
  unsigned char *p1 = (unsigned char*)image_data;
  unsigned char *p2 = (unsigned char*)image_data+1;
  unsigned char *p3 = (unsigned char*)image_data+2;

  size_t nb_bits = 8 * sizeof(char);
  size_t ct_bits = 0;
  int nb_pix = (nb_bits/3) + (nb_bits%3 != 0);
  for (int i = 0; i < nb_pix; ++i)
  {
    int idx = i * 3;
    if (ct_bits++ < nb_bits){
      p1[idx] = (p1[idx] - (p1[idx] % 2)) + (n%2);
      n = n >> 1;
    }
    if (ct_bits++ < nb_bits){
      p2[idx] = (p2[idx] - (p2[idx] % 2)) + (n%2);
      n = n >> 1;
    }
    if (ct_bits++ < nb_bits){
      p3[idx] = (p3[idx] - (p3[idx] % 2)) + (n%2);
      n = n >> 1;
    }
  }
  *offset += nb_pix;
}

char get_char_at(char *image_data, int *offset){
  image_data += *offset * 3;
  char *p1 = image_data;
  char *p2 = image_data+1;
  char *p3 = image_data+2;

  char val = 0;
  size_t  nb_bits = 8 * sizeof(char);
  size_t  ct_bits = 0;
  int nb_pix = (nb_bits / 3) + (nb_bits%3 != 0);
  for (int i = 0; i < nb_pix; ++i)
  {
    int idx = i * 3;
    if (ct_bits < nb_bits){
      val += (1 << ct_bits++) * ((p1[idx])%2);
    }
    if (nb_bits - ct_bits != 0){
      val += (1 << ct_bits++) * ((p2[idx])%2);
    }
    if (nb_bits - ct_bits != 0){
      val += (1 << ct_bits++) * ((p3[idx])%2);
    }
  }
  *offset += nb_pix;

  return val;
}

void stegano_hide(char *img_data, int fddata){
  struct stat st;
  int offset = 0;
  char c;

  if(fstat(fddata, &st) == -1){
    printf("fstat failed");
  }
  size_t data_size = st.st_size;

  set_int_at(img_data, &offset, data_size);

  while(read(fddata, &c, 1) == 1){
    set_char_at(img_data, &offset, c);
  }
}

void stegano_unhide(char *img_data, int fdout){
  char c;
  int offset = 0;
  int size = get_int_at(img_data, &offset);

  while(size-- != 0){
    c = get_char_at(img_data, &offset);
    write(fdout, &c, 1);
  }
}
