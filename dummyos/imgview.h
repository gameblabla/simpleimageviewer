#ifndef _IMGVIEW_H
#define _IMGVIEW_H
#include <stdint.h>
#include "n2DLib.h"
#include "control.h"
#include "graph.h"
int x = 0, y = 0;
unsigned char done;

int pict_x, pict_y;
int pict_w, pict_h;
unsigned char picture_mode = 0;

unsigned int fittoscreen_w, fittoscreen_h;

int limit;
unsigned char fit_to_screen;
unsigned char enable_clearing;
unsigned char zoom = 1;

#define BIG_PICTURE 1
#define SMALL_PICTURE 0

void Move_Image();
void Show_Image(int x, int y);
void Set_fit_to_screen(unsigned char set_mode);

#define MISC_ADDR 	0x900A0008
#define FOR_DELAY	0xAAAAA
#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT	240
#define SCREEN_SIZE		(SCREEN_WIDTH*SCREEN_HEIGHT)*2

#endif
