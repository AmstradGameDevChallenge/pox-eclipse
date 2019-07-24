#include <stdio.h>
#include <cpctelera.h>
#include "screen_utils.h"
#include "ctrl_char_codes.h"

char line_buffer[]="12345678901234567890123456789012345678901234567890123456789012345678901234567890";

void scnclr() {
    putchar(CLEAR_CHAR);
}

void locate_at(u8 row, u8 column) {
    char locate[]={LOCATE_CHAR, 0, 0, 0};
    locate[1]=column;
    locate[2]=row;
    puts(locate);
}

void print_at(u8 row, u8 column, char* string) {
    locate_at(row, column);
    puts(string);
}

void fill(u8 row, u8 column, u8 height, u8 width,  char character) {
    

    char* line_buffer_ptr = line_buffer;

    for (u8 i = 0; i < width; ++i){
        *(line_buffer_ptr++)=character;
    }
    *(line_buffer_ptr++)=0;

    for (u8 i = 0; i < height; ++i){
        print_at(row+i, column, line_buffer);

    }
}
