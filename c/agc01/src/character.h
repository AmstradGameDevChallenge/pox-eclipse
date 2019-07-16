#ifndef _CHARACTER_H
#define _CHARACTER_H

#include <cpctelera.h>
#include <stdbool.h>

typedef struct {
   i8 agility;
   i8 energy;
   i8 attack;
   i8 defense;
   i8 food;
   const char name[20];
   bool defending;
} character_stats;

void init_player( character_stats* );
void init_enemy( character_stats* );


bool is_dead( character_stats *stats );
void print_stats( character_stats* character );

#endif