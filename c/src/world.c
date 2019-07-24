#include <stdlib.h>
#include <stdio.h>
#include "world.h"
#include "character.h"
#include "util.h"
#include "screen_utils.h"


// World dimensions (latitude, longitude)
const u8 MIN_LAT = 0;
const u8 MAX_LAT = 15;
const u8 MIN_LON = 0;
const u8 MAX_LON = 15;

// Initial player location
const u8 PLAYER_INITIAL_LON = 7;
const u8 PLAYER_INITIAL_LAT = 7;

const char PLAYER_CHAR = 250;
const char FOG_CHAR = 127;

game_world world;

u16 fog[16];

inline bool is_visible(u16* fog, u8 lon, u8 lat)
{
    if (lat <= MAX_LAT || lon <= MAX_LON) {
        return (fog[lat] & (((u16)0x1)<<lon)) == 0;
    }
    else
    {
        return false;
    }
    
}

void make_visible(u16* fog, u8 lon, u8 lat)
{    
    fog[lat] =  fog[lat] & ~(((u16)0x1)<<lon);
}

void world_print_map() {
    char attrezzo[]={' ',0xAB,0xA0,0xB8};
    u8 player_lon = world.player_pos.lon;
    u8 player_lat = world.player_pos.lat;
    srand(0);
    for (u8 lat = MIN_LAT; lat <= MAX_LAT; ++lat  ) {
        locate_at(4+lat, 5);
        for (u8 lon = MIN_LON; lon <= MAX_LON; ++lon  ) {
            int scenery = rand();
            if (player_lat == lat && player_lon == lon) {
                putchar( PLAYER_CHAR);
            }
            else 
            {
                if (is_visible(fog, lon, lat)) {
                    putchar(attrezzo[((unsigned)scenery)%4] );
                }
                else
                {
                    putchar(FOG_CHAR);
                }
            }
        }
        printf("\r\n");
    }
}

void init_world() {    
    cpct_memset(fog, 0xff, sizeof(fog));
    init_player( &world.player );
    move_player_to(PLAYER_INITIAL_LON, PLAYER_INITIAL_LAT);
    world.enemies_count = 0;

    // Here we are going to populate our world
    // for(int enemy_count = 0 ; enemy_count < MAX_ENEMIES; ++enemy_count ) {
    //     printf("Spawning enemy %d\r\n", enemy_count);        
    // }
}

bool enemies_near(void) {
    return world.enemies_count > 0;
}

void spawn_enemy( void ) {
    if ( world.enemies_count < MAX_ENEMIES ) {
        init_enemy( &(world.enemies[world.enemies_count]) );
        wait_key();
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

u8 zone_content(u8 x, u8 y) {
    if (x == MAX_LON || 
        x == MIN_LON ||
        y == MAX_LAT ||
        y == MIN_LAT) {
            return CONTENT_OBSTACLE;
        } 
    else {
        srand(((u16)x)<<8+y);
        if (rand()%100 > 90 && cpct_rand() > 210) {
            return CONTENT_ENEMY;
        }
        else {
            return CONTENT_EMPTY;
        }
    }
}

bool move_player_to(u8 lon, u8 lat) {
    if (lon != world.player_pos.lon || lat != world.player_pos.lat) {
        world.player_pos.lon = lon;
        world.player_pos.lat = lat;
        make_visible(fog, lon, lat);
        return true;
    }
    else {
        return false;
    }
}

bool move_player(i8 lon_delta, i8 lat_delta) {
    u8 lon = world.player_pos.lon;
    u8 lat = world.player_pos.lat;
    bool moved = false;

    lon = clamp(lon + lon_delta, MIN_LON, MAX_LON);
    lat = clamp(lat + lat_delta, MIN_LAT, MAX_LAT);

    switch(zone_content(lon, lat)) {
        case CONTENT_EMPTY: {
            moved = move_player_to(lon, lat);
            break;
        }
        
        case CONTENT_OBSTACLE: {
            
            break;
        }
        
        case CONTENT_ITEM: {
            moved = move_player_to(lon, lat);
            break;
        }
        
        case CONTENT_ENEMY: {
            moved = move_player_to(lon, lat);
            spawn_enemy();
            break;
        }

        case CONTENT_CLUE: {
            moved = move_player_to(lon, lat);
            break;
        }

    }

    --world.player.energy;
    return( moved );
    
}