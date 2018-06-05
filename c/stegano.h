#ifndef STEGANO_H
#define STEGANO_H

#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/*
 * get an int hiden in image_data
 *
 * image_data:  base of image data
 * offset:      offset in pixel from base of image_data
 *
 * return:      the hiden int
 *
 * */
int get_int_at(char *image_data, int *offset);

/*
 * hide an int at chosen place in picture data
 *
 * image_data:  base of image data
 * offset:      offset in pixel from base of image_data
 * n:           value to set in image
 *
 * */
void set_int_at(char *image_data, int *offset, int n);

/*
 * hide a char at chosen place in picture data
 *
 * image_data:  base of image data
 * offset:      offset in pixel from base of image_data
 * n:           value to set in image
 *
 * */
void set_char_at(char *image_data, int *offset, char n);

/*
 * get a char hiden in image_data
 *
 * image_data:  base of image data
 * offset:      offset in pixel from base of image_data
 *
 * return:      the hiden char
 *
 * */
char get_char_at(char *image_data, int *offset);

void stegano_hide(char *img_data, int fddata);

void stegano_unhide(char *img_daat, int fdout);
#endif
