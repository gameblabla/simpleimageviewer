#ifndef INCLUDE_GRAFX
#define INCLUDE_GRAFX

#include <stdint.h>
#define GCNF_ADDR 	0xC0000018
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))


#define Texture_image const unsigned short

typedef int Fixed;

typedef struct
{
	int x;
	int y;
	int h;
	int w;
} Rect;


#define itofix(x) ((x) << 8)
#define fixtoi(x) ((x) >> 8)
#define fixdiv(x, y) (((x) << 8) / (y))
#define clamp(x, a, b) min(max(x, a), b)
#define sign(x) ((x) < 0)
#define fixsin(x) fixcos((x) - 64)

extern Fixed fixmul(Fixed x, Fixed y);
extern Fixed fixcos(Fixed angle);
extern void rotate(int x, int y, int cx, int cy, Fixed angle, Rect* out);
extern int sq(int);
extern Fixed fixsq(Fixed);
extern int cube(int);
extern Fixed fixcube(Fixed);

extern void Init_Video();
extern void Update_screen();
extern void Kill_Video();
extern void clearBufferB();
extern unsigned short getPixel(const unsigned short*, unsigned int, unsigned int);
extern void setPixelUnsafe(unsigned int, unsigned int, unsigned short);
extern void setPixel(unsigned int, unsigned int, unsigned short);
extern void setPixelRGB(unsigned int, unsigned int, unsigned char, unsigned char, unsigned char);
extern void drawSprite(Texture_image *src, int _x, int _y);
extern void drawSpritePart(Texture_image *src, int _x, int _y, unsigned short x_img, unsigned short y_img, unsigned short w, unsigned short h);
extern void drawSpriteScaled(Texture_image* source, short x, short y, unsigned short w, unsigned short h);

#define BUFF_BYTES_SIZE (320*240*2)

#endif
