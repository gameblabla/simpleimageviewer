#include "n2DLib.h"

/*             *
 *  Buffering  *
 *             */

unsigned short *BUFF_BASE_ADDRESS;

void Init_Video()
{
	BUFF_BASE_ADDRESS=*((void **)0xC0000010);
}

void Update_screen()
{
}

void Kill_Video()
{

}

static int strlen(const char *s)
{
    int len = 0;
    while(*s != 0) {
        s++;
        len++;
    }
    return len;
}

/*         *
 *  Maths  *
 *         */

 /*
Example:
2.5 * 3.5 :
	xdec = 128
	ydec = 128
	xint = 2
	yint = 3
2.5 * 3 = 8.75 :
	rdec = 192
	rint = 8
*/
 
inline Fixed fixmul(Fixed x, Fixed y)
{
	// x = (xint << 8)+ xdec, y = (yint << 8)+ ydec
	Fixed xdec = x & 0xff, ydec = y & 0xff, xint = x >> 8, yint = y >> 8;
	// Like (x * y) >> 8 ; a bit slower but without any risk of overflow (noticeable when squaring and cubing)
	return ((xint * yint) << 8) + xint * ydec + xdec * yint + ((xdec * ydec) >> 8);
}
 
Fixed fixcos(Fixed angle)
{
	static Fixed cosLUT[] = { 256, 255, 255, 255, 254, 254, 253, 252, 251, 249, 248, 246, 244, 243, 241, 238, 236, 234, 231, 228, 225, 222, 219, 216, 212, 209, 205, 201, 197, 193, 189, 185, 181, 176, 171, 167, 162, 157, 152, 147, 142, 136, 131, 126, 120, 115, 109, 103, 97, 92, 86, 80, 74, 68, 62, 56, 49, 43, 37, 31, 25, 18, 12, 6, 0, -6, -12, -18, -25, -31, -37, -43, -49, -56, -62, -68, -74, -80, -86, -92, -97, -103, -109, -115, -120, -126, -131, -136, -142, -147, -152, -157, -162, -167, -171, -176, -181, -185, -189, -193, -197, -201, -205, -209, -212, -216, -219, -222, -225, -228, -231, -234, -236, -238, -241, -243, -244, -246, -248, -249, -251, -252, -253, -254, -254, -255, -255, -255, -256, -255, -255, -255, -254, -254, -253, -252, -251, -249, -248, -246, -244, -243, -241, -238, -236, -234, -231, -228, -225, -222, -219, -216, -212, -209, -205, -201, -197, -193, -189, -185, -181, -176, -171, -167, -162, -157, -152, -147, -142, -136, -131, -126, -120, -115, -109, -103, -97, -92, -86, -80, -74, -68, -62, -56, -49, -43, -37, -31, -25, -18, -12, -6, 0, 6, 12, 18, 25, 31, 37, 43, 49, 56, 62, 68, 74, 80, 86, 92, 97, 103, 109, 115, 120, 126, 131, 136, 142, 147, 152, 157, 162, 167, 171, 176, 181, 185, 189, 193, 197, 201, 205, 209, 212, 216, 219, 222, 225, 228, 231, 234, 236, 238, 241, 243, 244, 246, 248, 249, 251, 252, 253, 254, 254, 255, 255, 255 };
	return cosLUT[angle & 0xff];
}


inline int sq(int x)
{
	return x * x;
}

inline Fixed fixsq(Fixed x)
{
	return fixmul(x, x);
}

inline int cube(int x)
{
	return x * x * x;
}

inline Fixed fixcube(Fixed x)
{
	return fixmul(fixmul(x, x), x);
}


/*            *
 *  Graphics  *
 *            */

void clearBufferB()
{
	int i;
	for(i = 0; i < 38400; i++)
		((unsigned int*)BUFF_BASE_ADDRESS)[i] = 0;
}

inline unsigned short getPixel(const unsigned short *src, unsigned int x, unsigned int y)
{
	if(x < src[0] && y < src[1])
		return src[x + y * src[0] + 3];
	else
		return 0;
}


inline void setPixelUnsafe(unsigned int x, unsigned int y, unsigned short c)
{
	*((unsigned short*)BUFF_BASE_ADDRESS + x + y * 320) = c;
}

inline void setPixel(unsigned int x, unsigned int y, unsigned short c)
{
	if(x < 320 && y < 240)
		*((unsigned short*)BUFF_BASE_ADDRESS + x + y * 320) = c;
}


void drawSprite(Texture_image *src, int _x, int _y)
{
	int x, y, 
	w = src[0] + _x, 
	h = src[1] + _y, c = 2;
	for(y = _y; y < h; y++)
	{
		for(x = _x; x < w; x++, c++)
		{
			setPixel(x, y, src[c]);
		}
		if(y > 239) break;
	}
}

void drawSpritePart(Texture_image *src, int _x, int _y, unsigned short x_img, unsigned short y_img, unsigned short w_img, unsigned short h_img)
{
	unsigned short c;
	int x, y, w = w_img + _x, h = h_img + _y, z = x_img, t = y_img;
	for(y = _y; y < h; y++, t++)
	{
		for(x = _x, z = x_img; x < w; x++, z++)
		{
			c = getPixel(src, z, t);
			setPixel(x, y, c);
			if(x > 319) break;
		}
		if(y > 239) break;
	}
}

void drawSpriteScaled(Texture_image* source, short x_img, short y_img, unsigned short w, unsigned short h)
{
	Fixed dx;
	Fixed dy;
	int x, y, _x , _y;
	dx = itofix(source[0]);
	dx /= w;
	dy = itofix(source[1]);
	dy /= h;
	_x = x_img + w / 2;
	_y = y_img + h / 2;
	Fixed tx = 0, ty = 0;
	unsigned short c;
	
	for(y = y_img - h / 2; y < _y; y++, ty += dy)
	{
		for(x = x_img - w / 2, tx = 0; x < _x; x++, tx += dx)
		{
			c = getPixel(source, fixtoi(tx), fixtoi(ty));
			setPixel(x, y, c);
			if(x > 319) break;
		}
		if(y > 239) break;
	}
}
