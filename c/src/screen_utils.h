#ifndef _SCREEN_UTILS
#define _SCREEN_UTILS

#include <cpctelera.h>
#include "ctrl_char_codes.h"


void scnclr();
void locate_at(u8 row, u8 column);
void print_at(u8 row, u8 column, char* string);
void fill(u8 row, u8 column, u8 height, u8 width,  char character);

#endif

