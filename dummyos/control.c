#include "key.h"

unsigned char pad_keys[8];

void Refresh_controls()
{
	unsigned char i;
	for(i=0;i<8;i++) pad_keys[i] = 0;
	
	if (KEY_8_PRESSED) pad_keys[0] = 1;
	else if (KEY_2_PRESSED) pad_keys[1] = 1;
	if (KEY_4_PRESSED) pad_keys[2] = 1;
	else if (KEY_6_PRESSED) pad_keys[3] = 1;
	
	if (KEY_SHIFT_PRESSED) pad_keys[4] = 1;
	if (KEY_CTRL_PRESSED) pad_keys[5] = 1;
	if (KEY_DEL_PRESSED) pad_keys[6] = 1;
	if (KEY_ESC_PRESSED) pad_keys[7] = 1;
}
