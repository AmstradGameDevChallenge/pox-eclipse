#include <stdbool.h>
#include <cpctelera.h>
#include <stdbool.h>
#include "util.h"
#include "screen_utils.h"


cpct_keyID INVALID_KEY = 255;

// returns max of two numbers a, b
i8 max(i8 a, i8 b) {
    if (a > b) { 
        return a; 
    }
    else {
        return b;
    }
}

// returns min of two numbers a, b
i8 min(i8 a, i8 b) {
        if (a <= b) { 
        return a; 
    }
    else {
        return b;
    }
}

// returns value "clamped" between given lower and upper limits
i8 clamp(i8 value, i8 lower_limit, i8 upper_limit) {
    if (value > upper_limit ) {
        return upper_limit;
    }
    else {
        if (value < lower_limit) {
            return lower_limit;
        }
        else {
            return value;
        }
    }
}

// Returns key pressed, if included in valid_keys array
// Otherwise, it loops forever
cpct_keyID key_pressed(cpct_keyID valid_keys[], u8 valid_keys_len) {
   bool valid_key_pressed = false;
   cpct_keyID key_caption = 255;

   do 
   {
        cpct_scanKeyboard(); 

        for (u8 i=0; i< valid_keys_len; ++i)  
        {            
            cpct_keyID current_key = valid_keys[i];
            
            if (cpct_isKeyPressed(current_key)) {
                valid_key_pressed = true;
                key_caption = current_key;
            }
        }    
   } while(!valid_key_pressed);

   return(key_caption);
}

void wait_key() {
    print_at(24, 1, "PRESS ANY KEY TO CONTINUE");
    do {
        cpct_scanKeyboard();
    } while ( !cpct_isAnyKeyPressed() );
}