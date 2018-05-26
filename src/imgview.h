#ifndef _IMGVIEW_H
#define _IMGVIEW_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "n2DLib.h"
#include "lodepng.h"
#include "control.h"
#include "ok_jpg.h"

int x = 0, y = 0;
unsigned char done;

uint16_t *picture;
short pict_x, pict_y;
short pict_w, pict_h;
short picture_mode = 0;

unsigned short fittoscreen_w, fittoscreen_h;

short limit;
unsigned char fit_to_screen;
unsigned char enable_clearing;
unsigned char zoom = 1;

#define BIG_PICTURE 1
#define SMALL_PICTURE 0

#define RGB565(r, g, b) (unsigned short)((r << 11) + (g << 5) + b)

#define BMP 0
#define PNG 1
#define JPG 2

int imgview(const char* filename);

int Load_Image(const char* path, unsigned char image_type);

void Move_Image();
void Show_Image(short x, short y);
void Sync();
void Set_fit_to_screen(unsigned char set_mode);

#endif
