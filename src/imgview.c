#include "imgview.h"

#ifdef NO_FILEBROWSER

int main(int argc, char* argv[])
{
	if (argc < 2)
		return 1;
	else
		imgview(argv[1]);
	return 0;
}

#endif

int imgview(const char* filename)
{
	unsigned char format_pict = 10;
	unsigned char error;
	done = 1;

	Init_Video();
	
	/* Detect file format according to its extension */
	if (strstr(filename, ".png") != 0 || strstr(filename, ".PNG") != 0)
		format_pict = PNG;
	else if (strstr(filename, ".jpg") != 0 || strstr(filename, ".JPG") != 0 || strstr(filename, ".JPEG") != 0 || strstr(filename, ".jpeg") != 0)
		format_pict = JPG;
	else if (strstr(filename, ".bmp") != 0 || strstr(filename, ".BMP") != 0)
		format_pict = BMP;
	
	error = Load_Image(filename, format_pict);
	if (error == 1)
		done = 0;
	
	while(done)
	{
		Move_Image();
		Update_screen();
		Sync();
	}
	
	if (picture != NULL) free(picture);
	Kill_Video();
	
	return 0;
}

unsigned short int rgb888Torgb565(unsigned int p1, unsigned int p2, unsigned int p3)
{
    unsigned int r, g, b;
    r = p1 >> 3 << 11;
    g = p2 >> 2 << 5;
    b = p3 >> 3;
    return (unsigned short int) (r | g | b);
}

int Load_Image(const char* path, unsigned char image_type)
{
	unsigned int i; 
	unsigned int w , h;
	unsigned char* image;
	unsigned int picture_size;
	FILE* fp;
	ok_jpg * imgj;
	/* If file doesn't exist then exit */
	if(access( path, F_OK ) == -1) 
	{
		printf("File does not exist !\n");
		return 1;
	}
	
	switch (image_type)
	{
		/*
		 * Uses n2DLib's built-in BMP loader.
		 * This only supports 24-bits files.
		 * Not a huge deal, since most bmp files out there are 24-bits.
		*/
		case BMP:
			picture_size = Getfilesize(path);
			if (picture_size == 0)
			return 1;
			picture = malloc(picture_size*2);
			if (!picture)
			return 1;
			picture = loadBMP(path, &w, &h);
		break;
		/* This uses LodePNG
		 * Decompressed image is 24-bits, then it is converted to RGB565.
		 * Most formats should be handled properly.
		*/
		case PNG:
			lodepng_decode24_file(&image, &w, &h, path);
			picture = malloc(((w * h) * sizeof(uint16_t))+2);
			if (!picture)
			return 1;
			picture[0] = w;
			picture[1] = h;
			for (i = 0; i < (w * h) - 2; i += 1) 
				picture[i+2] = rgb888Torgb565(image[i*3],image[i*3+1],image[i*3+2]);
			if (image != NULL) free(image);
		break;

		case JPG:
			/* OK_JPG Implementation, supports Progressive pictures. */
			fp = fopen(path, "rb");
			imgj = ok_jpg_read(fp, OK_JPG_COLOR_FORMAT_RGBA);
			fclose(fp);
			w = imgj->width;
			h = imgj->height;
			picture = malloc(((w * h) * 3)+2);
			picture[0] = w;
			picture[1] = h;
			uint8_t *dst = imgj->data;
			for (i = 0; i < (w * h) - 2; i += 1) 
			{
				picture[i+2] = rgb888Torgb565(dst[i*3],dst[i*3+1],dst[i*3+2]);
			}
			ok_jpg_free(imgj);
			
			/* uJpeg (thread-safe fork of NanoJPEG) implementation
			 * NanoJPEG does not seem to work very well...
			 * This does not support progressive
			 * */
			#ifdef UJPEG_IMPLEMENTATION
			ujImage jpgimg = ujCreate();
			ujSetChromaMode(jpgimg, UJ_CHROMA_MODE_FAST);
			jpgimg = ujDecodeFile(NULL, path);
			
			w = ujGetWidth(jpgimg);
			h = ujGetHeight(jpgimg);
			image = ujGetImage(jpgimg, NULL);
			
			picture = malloc(ujGetImageSize(jpgimg) + 2);
			picture[0] = ujGetWidth(jpgimg);
			picture[1] = ujGetHeight(jpgimg);
			
			for (i = 0; i < (picture[0] * picture[1])-2; i += 1) 
			{
				picture[i+2] = rgb888Torgb565(image[i*3],image[i*3+1],image[i*3+2]);
			}
			ujFree(jpgimg);
			#endif
			
			/* Smallest JPEG decoder but only supports baseline */
			#ifdef TINYJPEG_DECODER
			picture_size = Get_JPEG_size(path, &w, &h);
			image2 = malloc(picture_size);
			if (!image2)
			return 1;
			Load_JPEG(path, image2);
			picture = malloc(picture_size);
			if (!picture)
			return 1;
			picture[0] = w;
			picture[1] = h;
			for (i = 0; i < (w * h)-2; i += 1) 
				picture[i+2] = image2[i];
			if (image2 != NULL) free(image2);
			#endif
			
		break;
		
		default:
			printf("Format not supported\n");
			return 1;
		break;
	}
	
	pict_x = 0;
	pict_y = 0;
	pict_w = w;
	pict_h = h;
	
	/* Prevent user in BIG Picture mode to go outside the image, for speed reasons */
	if (pict_w > 320 || pict_h > 240)
	{
		picture_mode = BIG_PICTURE;
		Set_fit_to_screen(1);
	}
	else
	{
		picture_mode = SMALL_PICTURE;
		Set_fit_to_screen(0);
	}
	
	/* Pre-calculate aspect-ratio correction */
	if (pict_h > pict_w)
	{
		fittoscreen_h = pict_h / (pict_h / 240);
		fittoscreen_w = pict_w / (pict_h / 240);
	}
	else if (pict_w > pict_h)
	{
		fittoscreen_w = pict_w / (pict_w / 320);
		fittoscreen_h = pict_h / (pict_w / 320);
	}
	
	return 0;
}

/* Function that is responsible for detecting key pressing and handling them */
void Move_Image()
{
	static unsigned char time_key = 0;
	static unsigned char move_pict = 4;
	unsigned char tofit;
	
	Refresh_controls();
	
	if (pad_keys[7])
		done = 0;
		
	/* This moves the picture over the screen */
	if (pad_keys[0])
	{
		pict_y -= move_pict;
	}
	else if (pad_keys[1])
	{
		if (pict_y < limit) pict_y += move_pict;
	}
	if (pad_keys[2])
	{
		pict_x -= move_pict;
	}
	else if (pad_keys[3])
	{
		if (pict_x < limit) pict_x += move_pict;
	}
	
	if (time_key > 4)
	{
		if (pad_keys[4])
		{
			if (zoom < 4) zoom += 1;
			move_pict = 4*zoom;
			time_key = 0;
		}
		else if (pad_keys[5])
		{
			if (zoom > 1) zoom -= 1;
			move_pict = 4*zoom;
			time_key = 0;
		}
		
		/* Fit-to-screen */
		if (pad_keys[6])
		{
			if (fit_to_screen)
			tofit = 0;
			else 
			tofit = 1;
			Set_fit_to_screen(tofit);
			time_key = 0;
		}
	}
	
	if (time_key < 10) time_key++;
			
	Show_Image(pict_x, pict_y);	
}

/* The function to show the pict on screen */
void Show_Image(short x, short y)
{	
	static unsigned char time_clear = 0;

	if (enable_clearing)
	{
		time_clear++;
		if (time_clear > 3)
		{
			clearBufferB();
			time_clear = 0;
		}
	}

	if (picture_mode == BIG_PICTURE)
	{
		if (fit_to_screen)
		drawSpriteScaled(picture, 160+x, 120+y, fittoscreen_w*zoom, fittoscreen_h*zoom);
		else
		drawSpritePart(picture, 0, 0, -x, -y, 320, 240);
	}
	else
	{
		if (fit_to_screen)
		drawSpriteScaled(picture, (160+x), (120+y), 320, 240);
		else
		drawSpriteScaled(picture, (160+x), (120+y), pict_w*zoom, pict_h*zoom);
	}
}

void Set_fit_to_screen(unsigned char set_mode)
{
	if (set_mode ==  0)
	{
		fit_to_screen = 0;
		if (picture_mode == BIG_PICTURE)
		{
			limit = -4;
			enable_clearing = 0;
		}
		else
		{
			limit = 400;
			enable_clearing = 1;
		}
		zoom = 1;
	}
	else
	{
		limit = 400;
		fit_to_screen = 1;
		enable_clearing = 1;
	}
			
	pict_x = 0;
	pict_y = 0;
}

#ifndef _TINSPIRE
static void msleep(unsigned char milisec)
{
	struct timespec req={0};
	time_t sec=(unsigned short)(milisec/1000);

	milisec=milisec-(sec*1000);
	req.tv_sec=sec;
	req.tv_nsec=milisec*1000000L;

	while(nanosleep(&req,&req)==-1)
	continue;
}
#endif

/* Prevent app from going faster than 60 FPS */
void Sync()
{
	#ifndef _TINSPIRE
	Uint32 start;
	start = SDL_GetTicks();
	if(16 > SDL_GetTicks()-start) msleep(16-(SDL_GetTicks()-start));
	#endif
}
