#include <stdio.h>
#include <cpctelera.h>
#include "screen_utils.h"
#include "ctrl_char_codes.h"

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

