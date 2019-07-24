#ifndef _SCREEN_UTILS
#define _SCREEN_UTILS

#include <cpctelera.h>
#include "ctrl_char_codes.h"


void scnclr();
void locate_at(u8 row, u8 column);
void print_at(u8 row, u8 column, char* string);

#endif

