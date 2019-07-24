#ifndef _UTIL_H
#define _UTIL_H

#include <cpctelera.h>



// some basic comparisons
i8 max(i8, i8);
i8 min(i8, i8);
i8 clamp(i8 value, i8 lower_limit, i8 upper_limit);
//i8 scale(i8 value, i8 value_min, i8 value_max, i8 output_min, i8 output_max);
cpct_keyID key_pressed(cpct_keyID valid_keys[], u8 valid_keys_len);
void wait_key(void);

void println(char *string);

#endif