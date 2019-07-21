#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "character.h"
#include "util.h"

bool is_dead( character_stats *stats ) {
    return stats->energy <= 0;
}

void init_player( character_stats* character ) {
    character->agility  = 60;
    character->energy   = 100;
    character->attack   = 30;
    character->defense  = 15;
    strcpy(character->name, "Max");
    character->defending=false;
}

void init_enemy( character_stats* character ) {
    character->agility  = clamp(cpct_rand(), 10, 80);
    character->energy   = clamp(cpct_rand(), 10, 50);
    character->attack   = clamp(cpct_rand(), 1, 15);
    character->defense  = clamp(cpct_rand(), 8, 12);
    strcpy(character->name, "Mutard");
    character->defending=false;
}

void print_stats( character_stats* character ) {
    printf("Name: %s (HP:%d) (A:%d) (D:%d)\r\n", character->name, character->energy, character->attack, character->defense);
}