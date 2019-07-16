#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "character.h"

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
    character->agility  = 50;
    character->energy   = 90;
    character->attack   = 20;
    character->defense  = 10;
    strcpy(character->name, "Mutard");
    character->defending=false;
}

void print_stats( character_stats* character ) {
    printf("%s [%d] (a%d) (d%d)\r\n", character->name, character->energy, character->attack, character->defense);
}