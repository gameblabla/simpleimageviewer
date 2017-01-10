#ifndef _KEYS_H_
#define _KEYS_H_
#define KEY_ESC_PRESSED			is_any_key_pressed()?is_rc_pressed(6,7):0

#define KEY_0_PRESSED			is_any_key_pressed()?is_rc_pressed(0,7):0
#define KEY_1_PRESSED			is_any_key_pressed()?is_rc_pressed(1,7):0
#define KEY_2_PRESSED			is_any_key_pressed()?is_rc_pressed(6,4):0
#define KEY_3_PRESSED			is_any_key_pressed()?is_rc_pressed(1,3):0
#define KEY_4_PRESSED			is_any_key_pressed()?is_rc_pressed(2,7):0
#define KEY_5_PRESSED			is_any_key_pressed()?is_rc_pressed(5,6):0
#define KEY_6_PRESSED			is_any_key_pressed()?is_rc_pressed(2,3):0
#define KEY_7_PRESSED			is_any_key_pressed()?is_rc_pressed(3,7):0
#define KEY_8_PRESSED			is_any_key_pressed()?is_rc_pressed(6,6):0
#define KEY_9_PRESSED			is_any_key_pressed()?is_rc_pressed(3,3):0

#define KEY_ENTER_PRESSED			is_any_key_pressed()?is_rc_pressed(0,1):0
#define KEY_MENU_PRESSED			is_any_key_pressed()?is_rc_pressed(6,5):0
#define KEY_DOC_PRESSED			is_any_key_pressed()?is_rc_pressed(6,3):0
#define KEY_TAB_PRESSED			is_any_key_pressed()?is_rc_pressed(6,9):0
#define KEY_VAR_PRESSED			is_any_key_pressed()?is_rc_pressed(5,1):0
#define KEY_SHIFT_PRESSED			is_any_key_pressed()?is_rc_pressed(7,8):0
#define KEY_CTRL_PRESSED			is_any_key_pressed()?is_rc_pressed(7,9):0
#define KEY_ON_PRESSED			is_any_key_pressed()?is_rc_pressed(0,9):0
#define KEY_DEL_PRESSED			is_any_key_pressed()?is_rc_pressed(5,9):0

void (*memset)(void* dest, int val, int size) = (void*) 0x10170; // Boot1 3.0.0.99
void (*memcpy)(const void* dest, const void* src, int size) = (void*) 0xFFB0;
int (*is_any_key_pressed)()= (void*)0x19974;
int (*is_rc_pressed)(int row, int col) = (void*) 0x197D0;

#endif
