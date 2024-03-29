/**
 * POX-ECLIPSE
 * A post-apocalyptic journey
 * by @macarvajal
 * July 2019
 **/

#include <cpctelera.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "util.h"
#include "screen_utils.h"
#include "world.h"

typedef enum game_states {
    START_MENU     = 0,
    COMBAT         = 1,
    EXPLORATION    = 2,
    ENDED          = 3
};
 
enum game_states game_state;

void start_menu_state(void);
void combat_state(void);
void exploration_state(void);
void endgame_state(void);

void print_header(void);
void print_splash(void);

void player_action(character_stats *player, character_stats *enemy);
void enemy_action(character_stats *player, character_stats *enemy);
void player_explore_action(character_stats *player);
void resolve_attack(character_stats *foe, character_stats *attacker);
void control_max_min_energy(character_stats *character);



void main(void) {         

    game_state = START_MENU;
    while (true) {
        switch( game_state ) {
            case START_MENU: {                
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
    }
}


void start_menu_state(){
    // Let's start!
    init_world();
    print_splash();
    wait_key();
    scnclr();
    game_state = EXPLORATION;
}

bool player_wins(character_stats* enemy) {
    if ( is_dead(enemy) ) {
        println("Your opponent falls dead \r\n on the floor.\r\nThis time, you won!");
        remove_enemy();
        game_state = EXPLORATION;
        wait_key();
        scnclr();
        return true;
    }
    else {
        return false;
    }
}

bool enemy_wins(character_stats* player) {
    if ( is_dead(player) ) {
        println("You are severely injuried.\r\n You fall unconscious and\r\nyour rival kills you!");
        game_state = ENDED;
        wait_key();
        scnclr();
        return true;
    }
    else {
        return false;
    }
}

void combat_state() {
    character_stats *player = &(world.player);
    character_stats *enemy = get_enemy();

    // CLS
    scnclr();

    // Print stats
    print_stats(player);
    print_stats(enemy);
    player->defending = false;
    enemy->defending = false;

    if (player->agility > enemy->agility){
        println("Your agility allows you act earlier!");
        player_action(player, enemy);  
        if (!player_wins(enemy)) {
            enemy_action(player, enemy);
            enemy_wins(player);
        }    
    }
    else {            
        println("Enemy is faster than you!");
        enemy_action(player, enemy);
        if (!enemy_wins(player)) {
            player_action(player, enemy);
            player_wins(enemy);
        }   
    }
}

void exploration_state() {
    character_stats *player = &(world.player);

    // Print stats

    print_header();
    print_stats(player);        
    world_print_map();

    player_explore_action(player);
}

void endgame_state() {
    println("Game ended, will you play again?");
    game_state = START_MENU;
}

void print_header() {
    fill(1,1, 1, 40, ' ');
    locate_at(1, 1);
    printf("Location: %d> %d^\r\n", world.player_pos.lon, world.player_pos.lat);
}


void print_splash()
{
    //COLS:  1234567890123456789012345678901234567890
    scnclr();

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
    puts(string); 
    putchar('\r');
}

void player_explore_action(character_stats *player)
{
    cpct_keyID command_keys[] = {Key_Q, Key_A, Key_O, Key_P, Key_R};
    u8 command_keys_len = sizeof(command_keys)/sizeof(command_keys[0]);
    u8 energy_gained = 0;    
    bool moved = false;
    cpct_keyID key;
    fill(20,1, 4, 40, ' ');
    print_at(20, 1, "O, P, Q, A - Move. R - Rest.");
    key = key_pressed(command_keys, command_keys_len);
    switch(key) {
        case(Key_R): {
            print_at(21, 1,"You rest for some time...");
            energy_gained = max(0, min(cpct_rand(), 10));
            player->energy += energy_gained;
            control_max_min_energy(player);
            moved = false;
            break;
        }
        case (Key_Q): {
            moved = move_player( 0, -1);
            break;
        }

        case (Key_A): {
            moved = move_player( 0, 1);
            break;
        }

        case (Key_O): {
            moved = move_player(-1, 0);
            break;
        }

        case (Key_P): {
            moved = move_player( 1, 0);
            break;
        }
    }

    if (moved) {
        print_at(21, 1,"You explore the zone...");
        if (enemies_near()) {
            print_at(22, 1,"And you are attacked!!!");
            game_state = COMBAT;    
            wait_key();
            scnclr();
        }
        else {
            print_at(22, 1,"Nothing found");
        }
    }
}

void player_action(character_stats *player, character_stats *enemy)
{
    cpct_keyID command_keys[] = {Key_A, Key_D};
    u8 command_keys_len = 2;
    cpct_keyID key;

    println("Press key for next action...");
    println("A - Attack. D - Defend");
    
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
    if (player->agility < enemy->agility && cpct_rand() < 64) {
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
    character->energy = clamp(character->energy, 0, 100);
}