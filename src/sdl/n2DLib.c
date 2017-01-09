#include "n2DLib.h"

/*             *
 *  Buffering  *
 *             */
#define min(X,Y) (((X) < (Y)) ? (X) : (Y))
#define max(X,Y) (((X) > (Y)) ? (X) : (Y))
SDL_Surface *screen;

void Init_Video()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_ShowCursor(0);
	screen = SDL_SetVideoMode(320, 240, 16, 
	#ifdef GCW0
	SDL_HWSURFACE | SDL_TRIPLEBUF
	#else
	SDL_SWSURFACE
	#endif
	);
}

void Update_screen()
{
	SDL_Flip(screen);
}

void Kill_Video()
{
	SDL_FreeSurface(screen);
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	SDL_Quit();
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
 
Fixed fixmul(Fixed x, Fixed y)
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

void rotate(int x, int y, int cx, int cy, Fixed angle, Rect *out)
{
	x -= cx;
	y -= cy;
	out->x = fixtoi(fixmul(itofix(x), fixcos(angle)) + fixmul(itofix(y), fixsin(angle))) + cx;
	out->y = fixtoi(fixmul(itofix(x), -fixsin(angle)) + fixmul(itofix(y), fixcos(angle))) + cy;
}

void getBoundingBox(int x, int y, int w, int h, int cx, int cy, Fixed angle, Rect *out)
{
	Rect tl, tr, bl, br;
	rotate(x, y, cx, cy, angle, &tl);
	rotate(x + w, y, cx, cy, angle, &tr);
	rotate(x, y + h, cx, cy, angle, &bl);
	rotate(x + w, y + h, cx, cy, angle, &br);
	out->x = min(min(min(tl.x, tr.x), bl.x), br.x);
	out->y = min(min(min(tl.y, tr.y), bl.y), br.y);
	out->w = max(max(max(tl.x, tr.x), bl.x), br.x) - out->x;
	out->h = max(max(max(tl.y, tr.y), bl.y), br.y) - out->y;
}

Fixed fixsq(Fixed x)
{
	return fixmul(x, x);
}

int cube(int x)
{
	return x * x * x;
}

Fixed fixcube(Fixed x)
{
	return fixmul(fixmul(x, x), x);
}

// Uses Lagrange's interpolation polynomial
// Returns the next t parameter so it can be fed back to the function the next call
int interpolatePathFloat(int curT, float _x[], float _y[], int _t[], int nb, Rect *out)
{
	float factor, rx = 0., ry = 0.;
	int i, j;
	
	for(i = 0; i < nb; i++)
	{
		factor = 1.;
		for(j = 0; j < nb; j++)
			if(i != j)
				factor *= (float)(curT - _t[j]) / (_t[i] - _t[j]);
		rx += _x[i] * factor;
		ry += _y[i] * factor;
	}
	
	out->x = (int)rx;
	out->y = (int)ry;
	return curT + 1;
}

/*            *
 *  Graphics  *
 *            */

void clearBufferB()
{
	int i;
	for(i = 0; i < 38400*2; i++)
		((unsigned short*)screen->pixels )[i] = 0;
}


inline unsigned short getPixelUnsafe(Texture_image *src, unsigned int x, unsigned int y)
{
	return src[x + y * src[0] + 3];
}

unsigned short getPixel(Texture_image *src, unsigned int x, unsigned int y)
{
	if(x < src[0] && y < src[1])
		return src[x + y * src[0] + 3];
	else
		return 0;
}

inline void setPixelUnsafe(unsigned int x, unsigned int y, unsigned short c)
{
	*((unsigned short*)screen->pixels + x + y * 320) = c;
}

inline void setPixel(unsigned int x, unsigned int y, unsigned short c)
{
	if(x < 320 && y < 240)
		*((unsigned short*)screen->pixels + x + y * 320) = c;
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
	Fixed dx = itofix(source[0]) / w;
	Fixed dy = itofix(source[1]) / h;
	int x, y, _x = x_img + w / 2, _y = y_img + h / 2;
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

void drawSpriteRotated(Texture_image* source, const Rect* sr, const Rect* rc, Fixed angle)
{
	Rect defaultRect = { source[0] / 2, source[1] / 2, 0, 0 };
	Rect fr;
	unsigned short currentPixel;
	Fixed dX = fixcos(angle), dY = fixsin(angle);
	
	if(rc == NULL)
		rc = &defaultRect;
	
	getBoundingBox(-rc->x, -rc->y, source[0], source[1], 0, 0, angle, &fr);
	fr.x += sr->x;
	fr.y += sr->y;
	fr.w += fr.x;
	fr.h += fr.y;
	
	Rect cp, lsp, cdrp;
	
	// Feed fixed-point to get fixed-point
	rotate(itofix(fr.x - sr->x), itofix(fr.y - sr->y), 0, 0, -angle, &lsp);
	
	for(cp.y = fr.y; cp.y <= fr.h; cp.y++)
	{
		cdrp.x = lsp.x;
		cdrp.y = lsp.y;
		
		for(cp.x = fr.x; cp.x <= fr.w; cp.x++)
		{
			currentPixel = getPixel(source, fixtoi(cdrp.x) + rc->x, fixtoi(cdrp.y) + rc->y);
			setPixel(cp.x, cp.y, currentPixel);
			cdrp.x += dX;
			cdrp.y += dY;
		}
		
		lsp.x -= dY;
		lsp.y += dX;
	}
}

unsigned int Getfilesize(const char *path)
{
	unsigned int size, width, height;
	FILE *temp = fopen(path, "rb");
	if(!temp) return 0;
	// Get the 4-bytes pixel width and height, situated respectively at 0x12 and 0x16
	fseek(temp, 0x12, SEEK_SET);
	width = fgetc(temp) | (fgetc(temp) << 8) | (fgetc(temp) << 16) | (fgetc(temp) << 24);
	fseek(temp, 0x16, SEEK_SET);
	height = fgetc(temp) | (fgetc(temp) << 8) | (fgetc(temp) << 16) | (fgetc(temp) << 24);
	size = (width * height) + 3; // include header
	fclose(temp);
	return size;
}


unsigned short* loadBMP(const char *path, unsigned int* wid, unsigned int* hei)
{
	int size, width, height, offset, i, j, padding;
	uint16_t *returnValue;
	FILE *temp = fopen(path, "rb");
	
	if(!temp) return NULL;
	// Check if the file's 2 first char are BM (indicates bitmap)
	if(!(fgetc(temp) == 0x42 && fgetc(temp) == 0x4d))
	{
		printf("Image is not a bitmap\n");
		fclose(temp);
		return NULL;
	}
	
	// Check if the file is in 24 bpp
	fseek(temp, 0x1c, SEEK_SET);
	if(fgetc(temp) != 24)
	{
		printf("Wrong format : bitmap must use 24 bpp\n");
		fclose(temp);
		return NULL;
	}
	
	// Get the 4-bytes pixel width and height, situated respectively at 0x12 and 0x16
	fseek(temp, 0x12, SEEK_SET);
	width = fgetc(temp) | (fgetc(temp) << 8) | (fgetc(temp) << 16) | (fgetc(temp) << 24);
	//fseek(temp, 0x16, SEEK_SET);
	height = fgetc(temp) | (fgetc(temp) << 8) | (fgetc(temp) << 16) | (fgetc(temp) << 24);
	size = width * height + 3; // include header
	
	// Gets the 4-bytes offset to the start of the pixel table, situated at 0x0a
	fseek(temp, 0x0a, SEEK_SET);
	offset = fgetc(temp) | (fgetc(temp) << 8) | (fgetc(temp) << 16) | (fgetc(temp) << 24);
	
	fseek(temp, offset, SEEK_SET);
	
	returnValue = malloc(size * sizeof(unsigned short));
	if(!returnValue)
	{
		printf("Couldn't allocate memory\n");
		fclose(temp);
		return NULL;
	}
	returnValue[0] = width;
	returnValue[1] = height;
	padding = 4 - (width * 3 - (width * 3 / 4) * 4);
	padding %= 4;
	for(j = height - 1; j >= 0; j--)
	{
		for(i = 0; i < width; i++)
			returnValue[j * width + i + 2] = (unsigned short)((fgetc(temp) >> 3) | ((fgetc(temp) >> 2) << 5) | ((fgetc(temp) >> 3) << 11));
		// Skip padding for widths that are not a multiple of 4
		for(i = 0; i < padding; i++)
			fgetc(temp);
	}

	*wid = width;
	*hei = height;
	return returnValue;
}
