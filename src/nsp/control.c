#include <libndls.h>

unsigned char pad_keys[8];

void Refresh_controls()
{
	unsigned char i;
	for(i=0;i<8;i++) pad_keys[i] = 0;
	
	if (isKeyPressed(KEY_NSPIRE_UP)) pad_keys[0] = 1;
	else if (isKeyPressed(KEY_NSPIRE_DOWN)) pad_keys[1] = 1;
	if (isKeyPressed(KEY_NSPIRE_LEFT)) pad_keys[2] = 1;
	else if (isKeyPressed(KEY_NSPIRE_RIGHT)) pad_keys[3] = 1;
	
	if (isKeyPressed(KEY_NSPIRE_MINUS) || isKeyPressed(KEY_NSPIRE_SHIFT)) pad_keys[4] = 1;
	if (isKeyPressed(KEY_NSPIRE_PLUS) || isKeyPressed(KEY_NSPIRE_CTRL)) pad_keys[5] = 1;
	if (isKeyPressed(KEY_NSPIRE_DEL)) pad_keys[6] = 1;
	if (isKeyPressed(KEY_NSPIRE_ESC)) pad_keys[7] = 1;
}
