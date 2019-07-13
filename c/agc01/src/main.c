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
   bool defending;
} character_stats;

void print_splash(void);
void println(char *string);
cpct_keyID key_pressed(cpct_keyID valid_keys[], u8 valid_keys_len);
void wait_key(void);
void player_action(character_stats *player, character_stats *enemy);
void enemy_action(character_stats *player, character_stats *enemy);
void resolve_attack(character_stats *foe, character_stats *attacker);
void control_max_min_energy(character_stats *character);
i8 max(i8, i8);
i8 min(i8, i8);
bool died(character_stats *stats);


void main(void) {

    bool game_running = false;
    character_stats player = {.agility=60, .energy = 100, .attack = 30, .defense = 15, .name="Max", .defending=false};
    character_stats enemy = {.agility= 50, .energy = 90, .attack = 20, .defense = 10, .name="mutard", .defending=false};  

    while (true) {
        // Let's start!
        print_splash();  

        wait_key();
        game_running = true;
        while (game_running) {
            // CLS
            putchar(12);

        // Print stats
            printf("PLAYER [%d] (a%d) (d%d)\r\n", player.energy, player.attack,   player.defense);
            printf("ENEMY  [%d] (a%d) (d%d)\r\n", enemy.energy, enemy.attack, enemy.defense);
            player.defending = false;
            enemy.defending = false;

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
            
            if ( died(&player) ) {
                println("You are severely injuried.\r\n You fall unconscious and\r\nyour rival kills you!");
                game_running = false;
            }
            
            if ( died(&enemy) ) {
                println("Your last blow makes your enemy\r\nwalk back some steps.\r\nThen falls dead on the floor.");                
                game_running = false;
            }
            
            wait_key();
        }

        println("Game ended, will you play again?");
        wait_key();
    }
}

bool died(character_stats *stats) {
    return stats->energy <= 0;
}

void print_splash()
{
    //COLS:  1234567890123456789012345678901234567890
    putchar(12);
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
            resolve_attack(enemy, player);
            break;
        }
        case Key_D: {            
            printf("Player %s defends of %s...\r\n", player->name, enemy->name );
            player->defending = true;
            break;
        }
    }
}

void enemy_action(character_stats *player, character_stats *enemy)
{
    // ENEMY DECIDES
    if (cpct_rand() < 64) {
        printf("Enemy %s defends of %s...\r\n", enemy->name, player->name );         
        enemy->defending=true;

    } else {
        printf("Enemy %s attacks %s...\r\n", enemy->name, player->name );
        resolve_attack(player, enemy); 
        control_max_min_energy(player);
    }
}

void resolve_attack(character_stats *foe, character_stats *attacker) {
    i8 attack_strength = cpct_rand() % attacker->attack;
    i8 damage = foe->defending? max(0, attack_strength-foe->defense) : attack_strength;
    
    foe->energy -= damage;
    control_max_min_energy(foe);
    if (damage > 0){
        printf("%s makes %d points of damage!\r\n", attacker->name, damage);
    }
    else {
        printf("%s's attack failed!\r\n", attacker->name, damage);
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

i8 max(i8 a, i8 b) {
    if (a > b) { 
        return a; 
    }
    else {
        return b;
    }
}

i8 min(i8 a, i8 b) {
        if (a <= b) { 
        return a; 
    }
    else {
        return b;
    }
}