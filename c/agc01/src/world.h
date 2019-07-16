
#ifndef WORLD_H
#define WORLD_H

#include "character.h"
#define MAX_ENEMIES 5

typedef struct {
    character_stats player;
    character_stats enemies[MAX_ENEMIES]; 
    i8 enemies_count;
} game_world;


void init_world();
void spawn_enemy( void );
character_stats *get_enemy( void );
void remove_enemy( void );

void move_player(character_stats *player);
extern game_world world;
#endif