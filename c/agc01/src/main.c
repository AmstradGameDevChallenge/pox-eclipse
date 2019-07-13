/**
 * SUPER ULTRA RPG GAME
 * by @hec_linares
 * July 2019
 **/

#include <cpctelera.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define NEW_LINE "\r\n"
cpct_keyID INVALID_KEY = 255;

typedef struct {
   i8 agility;
   i8 energy;
   i8 attack;
   i8 defense;
   const char* name;
} character_stats;

void print_splash(void);
void println(char *string);
cpct_keyID key_pressed(cpct_keyID valid_keys[], u8 valid_keys_len);
void wait_key(void);
void player_action(character_stats *player, character_stats *enemy);
void enemy_action(character_stats *player, character_stats *enemy);
void control_max_min_energy(character_stats *character);


void main(void) {

    character_stats player = {.agility=60, .energy = 100, .attack = 30, .defense = 15, .name="Max"};
    character_stats enemy = {.agility= 50, .energy = 90, .attack = 20, .defense = 10, .name="mutard"};  


    // Let's start!
    print_splash();  

    wait_key();
        
    while (1) {
         // CLS
         putchar(12);

       // Print stats
        printf("PLAYER [%d] (a%d) (d%d)\r\n", player.energy, player.attack,   player.defense);
        printf("ENEMY  [%d] (a%d) (d%d)\r\n", enemy.energy, enemy.attack, enemy.defense);

        if (player.agility > enemy.agility){
            println("Your agility allows you act before");
            player_action(&player, &enemy);
            enemy_action(&player, &enemy);
            
        }
        else {            
            println("Enemy is faster than you!");
            enemy_action(&player, &enemy);
            player_action(&player, &enemy);
        }
        
        wait_key();
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
    
}

void println(char *string)
{
    printf(string);
    printf(NEW_LINE);
}

cpct_keyID key_pressed(cpct_keyID valid_keys[], u8 valid_keys_len) 
{
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


void player_action(character_stats *player, character_stats *enemy)
{
    cpct_keyID command_keys[] = {Key_A, Key_D};
    u8 command_keys_len = 2;
    cpct_keyID key;

    println("Press key for next move...");
    
    key = key_pressed(command_keys, command_keys_len);
    switch( key ) {
        case Key_A: {            
            printf("Player %s attacts %s...\r\n", player->name, enemy->name );        
            enemy->energy -= player->attack;
            control_max_min_energy(enemy);
            break;
        }
        case Key_D: {            
            printf("Player %s defends of %s...\r\n", player->name, enemy->name );
            player->energy += player->defense;
            control_max_min_energy(player);
            break;
        }
    }
}

void enemy_action(character_stats *player, character_stats *enemy)
{
    // ENEMY DECIDE
    if (cpct_rand() < 64) {
        printf("Enemy %s defends of %s...\r\n", enemy->name, player->name );
        enemy->energy += enemy->defense;        
        control_max_min_energy(enemy);

    } else {
        printf("Enemy %s attacks %s...\r\n", enemy->name, player->name );
        player->energy -= enemy->attack;
        control_max_min_energy(player);
    }
}

void control_max_min_energy(character_stats *character) {
        if (character->energy > 100) {
            character->energy = 100;
        }
        else {
            if (character->energy <= 0) {
                character->energy = 0;
            }
        }
}

void wait_key() {
    cpct_keyID wait_keys[] = {Key_Enter};
    u8 wait_keys_len = 1;
    cpct_keyID key; 
    
    println("PRESS ENTER TO CONTINUE");
    do {
        key = key_pressed(wait_keys, wait_keys_len);
    } while (key == INVALID_KEY);
}