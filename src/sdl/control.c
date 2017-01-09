#include <SDL.h>

unsigned char pad_keys[8];

void Refresh_controls()
{
	unsigned char i;
	Uint8* keystate = SDL_GetKeyState(NULL);
	
	for(i=0;i<8;i++) pad_keys[i] = 0;
	
	if (keystate[SDLK_UP]) pad_keys[0] = 1;
	else if (keystate[SDLK_DOWN]) pad_keys[1] = 1;
	if (keystate[SDLK_LEFT]) pad_keys[2] = 1;
	else if (keystate[SDLK_RIGHT]) pad_keys[3] = 1;
	
	if (keystate[SDLK_LCTRL]) pad_keys[4] = 1;
	if (keystate[SDLK_LALT]) pad_keys[5] = 1;
	if (keystate[SDLK_LSHIFT]) pad_keys[6] = 1;
	if (keystate[SDLK_ESCAPE]) pad_keys[7] = 1;
	
	SDL_PumpEvents();
}
