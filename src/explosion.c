// // explosions

#include <genesis.h>
#include <resources.h>

#include "main.h"
#include "explosion.h"

void spawnExplosion(fix16 x, fix16 y, s16 priority){
	s16 i = -1;
	for(s16 j = 0; j < EXPLOSION_LIMIT; j++) if(!explosions[j].active && i == -1) i = j;
	explosions[i].active = TRUE;
	explosions[i].clock = 0;
	explosions[i].image = SPR_addSprite(&explosion,
		fix16ToInt(fix16Sub(x, EXPLOSION_OFFSET)) + GAME_X,
		fix16ToInt(fix16Sub(y, EXPLOSION_OFFSET)) + GAME_Y,
		TILE_ATTR(PAL1, 1, 0, 0));
}

void updateExplosion(s16 i){
	if(explosions[i].clock >= EXPLOSION_LIMIT){
		explosions[i].active = FALSE;
		SPR_releaseSprite(explosions[i].image);
	}
	explosions[i].clock++;
}

void updateExplosions(){
	for(s16 i = 0; i < EXPLOSION_LIMIT; i++) if(explosions[i].active) updateExplosion(i);
}