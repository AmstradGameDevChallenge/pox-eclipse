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
#define START_MENU  0
#define COMBAT      1
#define EXPLORATION 2
#define ENDED       3
 
cpct_keyID INVALID_KEY = 255;

typedef struct {
   i8 agility;
   i8 energy;
   i8 attack;
   i8 defense;
   const char* name;
   bool defending;
} character_stats;

u8 game_state;
void start_menu_state(void);
void combat_state(void);
void exploration_state(void);
void endgame_state(void);

character_stats player; 
character_stats enemy;

void print_splash(void);
void println(char *string);
cpct_keyID key_pressed(cpct_keyID valid_keys[], u8 valid_keys_len);
void wait_key(void);
void player_action(character_stats *player, character_stats *enemy);
void enemy_action(character_stats *player, character_stats *enemy);
void player_explore_action(character_stats *player);
void resolve_attack(character_stats *foe, character_stats *attacker);
void control_max_min_energy(character_stats *character);
i8 max(i8, i8);
i8 min(i8, i8);
bool died(character_stats *stats);

void create_player() {
    player.agility=60;
    player.energy = 100;
    player.attack = 30;
    player.defense = 15;
    player.name="Max";
    player.defending=false;
}

void create_enemy() {
    enemy.agility= 50;
    enemy.energy = 90;
    enemy.attack = 20;
    enemy.defense = 10;
    enemy.name="a mutard";
    enemy.defending=false;  
}

void main(void) {        
    game_state = 0;
    while (true) {
        switch( game_state ) {
            case START_MENU: {
                create_player();
                start_menu_state();
                break;
            }
            case COMBAT: {                
                combat_state();
                break;
            }
            case EXPLORATION: {
                exploration_state();
                break;
            }
            case ENDED: {
                endgame_state();
                break;
            }
        }
        wait_key();
    }
}


void start_menu_state(){
    // Let's start!
    print_splash();  
    game_state = EXPLORATION;
}

void combat_state() {
    bool fighting = true;

    // CLS
    putchar(12);

    // Print stats
    printf("PLAYER [%d] (a%d) (d%d)\r\n", player.energy, player.attack,   player.defense);
    printf("ENEMY  [%d] (a%d) (d%d)\r\n\n\n", enemy.energy, enemy.attack, enemy.defense);
    printf("a - Attack. d - Defend\r\n");
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
        fighting = false;
        game_state = ENDED;
    }
    
    if ( died(&enemy) ) {
        println("Your last blow makes your enemy\r\nwalk back some steps.\r\nThen falls dead on the floor.");                
        fighting = false;
        game_state = EXPLORATION;
    }        
}

void exploration_state() {
    // Print stats
    putchar(12);
    printf("PLAYER [%d] (a%d) (d%d)\r\n", player.energy, player.attack,   player.defense);
    printf("m - Move. r - Rest. \r\n");
    player_explore_action(&player);
}

void endgame_state() {
    println("Game ended, will you play again?");
    game_state = START_MENU;
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
    println("from the Big Boum-Boum. now the earth");
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

void player_explore_action(character_stats *player)
{
    cpct_keyID command_keys[] = {Key_M, Key_R};
    u8 command_keys_len = 2;
    u8 energy_gained = 0;    
    cpct_keyID key;

    key = key_pressed(command_keys, command_keys_len);
    switch(key) {
        case(Key_R): {
            println("You rest for some time...");
            energy_gained = max(0, min(cpct_rand(), 10));
            player->energy += energy_gained;
            control_max_min_energy(player);
            break;
        }
        case (Key_M): {
            println("You explore the zone...");
            if (cpct_rand()>100) {
                create_enemy();
                printf("And find %s!!!", enemy.name);
                
                game_state = COMBAT;
            }
            break;
        }
    }
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
        printf("%s defends of %s...\r\n", enemy->name, player->name );         
        enemy->defending=true;

    } else {
        printf("%s attacks %s...\r\n", enemy->name, player->name );
        resolve_attack(player, enemy);
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