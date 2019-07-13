/**
 * SUPER ULTRA RPG GAME
 * by @hec_linares
 * July 2019
 **/

#include <cpctelera.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define NEW_LINE "\r\n"

typedef struct {
   u8 energy;
   u8 attack;
   u8 defense;
   const char* name;
} character_stats;

void print_splash();
void println(char *string);

void main(void) {

    character_stats player = {.energy = 100, .attack = 30, .defense = 15, .name="Max"};
    character_stats enemy = {.energy = 90, .attack = 20, .defense = 10, .name="mutard"};  
   
   // Let's start!
   print_splash();   
   
   // Is enter pressed?
   do {
        cpct_scanKeyboard();        
   }
   while (!cpct_isKeyPressed(Key_Enter));

   while (1) {
           
        // CLS
        putchar(12);

      // Print stats
      printf("PLAYER [%d] (a%d) (d%d)\r\n", player.energy,   player.attack,   player.defense);
      printf("ENEMY  [%d] (a%d) (d%d)\r\n", enemy.energy, enemy.attack, enemy.defense);
      
      // Re scan keyboard
      cpct_scanKeyboard();
      
      while (!cpct_isKeyPressed(Key_A) && !cpct_isKeyPressed(Key_D)) {
         cpct_scanKeyboard();
      }
         
      // PLAYER ATTACKS!
      if (cpct_isKeyPressed(Key_A)) {
         enemy.energy -= player.attack;
      } else {
         // PLAYER DEFENDS!
         if (cpct_isKeyPressed(Key_D)) {
            player.energy += player.defense;
         }
      }
      
      // ENEMY DECIDE
      if (cpct_rand() < 64) {
         enemy.energy += enemy.defense;
      } else {
         player.energy -= player.attack;
      }
   }
}

void print_splash()
{
    //COLS:  1234567890123456789012345678901234567890
    println("POX ECLIPSE");
    println("===========");
    println("Welcome to the end of the world!");
    println("Nobody knows how many years have passed");
    println("from the Big Boum Boum, now the earth");
    println("is poisoned and everyone's life is");
    println("blood and fire. Day after day, you have");
    println("managed to survive in this barren land,");
    println("paying a high price. Friends and family"); 
    println("are now dead.");
    println("You are alone, and you feel you must");
    println("keep yourself alive. Deeply buried in");
    println("your soul, a tiny spark of hope");
    println("brights");
    println("");
    println("PRESS ENTER TO START");
}

void println(char *string)
{
    printf(string);
    printf(NEW_LINE);
}