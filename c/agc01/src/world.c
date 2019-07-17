#include <stdlib.h>
#include "world.h"
#include "character.h"
#include "util.h"

// World dimensions
const i8 MIN_Y = -12;
const i8 MAX_Y = 12;
const i8 MIN_X = -12;
const i8 MAX_X = 12;

game_world world;

void init_world() {
    init_player( &world.player );
    world.player_pos.x=0;
    world.player_pos.y=0;
    world.enemies_count = 0;
}

bool enemies_near(void) {
    return world.enemies_count > 0;
}

void spawn_enemy( void ) {
    if ( world.enemies_count < MAX_ENEMIES ) {
        init_enemy( &(world.enemies[world.enemies_count]) );
        ++world.enemies_count;        
    }
}

void remove_enemy( void ) {    
    if ( world.enemies_count > 0 ) {
        --world.enemies_count;
    }
}

character_stats *get_enemy() {
    return &(world.enemies[0]);
}

u8 zone_content(i8 x, i8 y) {
    if (cpct_rand()>200) {
        spawn_enemy();
        return CONTENT_ENEMY;
    }
    else {
        return CONTENT_EMPTY;
    }
}

void move_player_to(i8 pos_x, i8 pos_y) {
    world.player_pos.x = pos_x;
    world.player_pos.y = pos_y;
}

void move_player(i8 x_mov, i8 y_mov) {
    i8 pos_x = world.player_pos.x;
    i8 pos_y = world.player_pos.y;
    
    pos_x = clamp(pos_x + x_mov, MIN_X, MAX_X);
    pos_y = clamp(pos_y + y_mov, MIN_Y, MAX_Y);

    switch(zone_content(pos_x, pos_y)) {
        case CONTENT_EMPTY: {
            move_player_to(pos_x, pos_y);
            break;
        }
        
        case CONTENT_OBSTACLE: {
            move_player_to(pos_x, pos_y);
            break;
        }
        
        case CONTENT_ITEM: {
            move_player_to(pos_x, pos_y);
            break;
        }
        
        case CONTENT_ENEMY: {
            move_player_to(pos_x, pos_y);
            spawn_enemy();
            break;
        }

        case CONTENT_CLUE: {
            move_player_to(pos_x, pos_y);
            break;
        }

    }

    --world.player.energy;
    
}