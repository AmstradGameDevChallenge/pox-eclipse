#ifndef _WINDOWS
#define _WINDOWS

#include <cpctelera.h>

typedef struct  {
    u8 column;
    u8 row;
    u8 width;
    u8 height;
} window;

void show_window(window*);

#endif