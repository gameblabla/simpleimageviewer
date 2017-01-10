#include "imgview.h"


int main(void)
{
	done = 1;

	Init_Video();
	
	pict_x = 0;
	pict_y = 0;
	pict_w = picture[0];
	pict_h = picture[1];
	
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
	
	while(done)
	{
		Move_Image();
	}
	
	Kill_Video();
	
	*((volatile uint32_t *)MISC_ADDR) = 2;
	
	return 0;
}

/* Function that is responsible for detecting key presses and handling them */
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
void Show_Image(int x, int y)
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
