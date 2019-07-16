#include <stdlib.h>
#include "world.h"
#include "character.h"

game_world world;

void init_world() {    
    init_player( &world.player );
    world.enemies_count = -1;
}

void spawn_enemy( void ) {
    if ( world.enemies_count < MAX_ENEMIES ) {
        ++world.enemies_count;
        init_enemy( &(world.enemies[world.enemies_count]) );
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

void move_player(character_stats *player) {
    --player->energy;
}