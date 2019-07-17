#ifndef _WORLD_H
#define _WORLD_H

#include <stdbool.h>
#include "character.h"
#define MAX_ENEMIES 5

#define CONTENT_EMPTY       0
#define CONTENT_OBSTACLE    1
#define CONTENT_ITEM        2
#define CONTENT_ENEMY       3
#define CONTENT_CLUE        4

typedef struct {
    i8 x;
    i8 y;
} location;

typedef struct {
    character_stats player;
    character_stats enemies[MAX_ENEMIES]; 
    i8 enemies_count;
    location player_pos;    
} game_world;

void init_world( void );
void destroy_world( void );

bool enemies_near( void );
void spawn_enemy( void );
character_stats *get_enemy( void );
void remove_enemy( void );

u8 zone_content(i8 x, i8 y);

void move_player(i8 x_mov, i8 y_mov);
void move_player_to(i8 pos_x, i8 pos_y);

extern game_world world;

#endif