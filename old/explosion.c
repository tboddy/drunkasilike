// // explosions

#include <genesis.h>
#include <resources.h>

#include "main.h"
#include "entities.h"
#include "explosion.h"

void spawnExplosion(fix16 x, fix16 y){
	struct spawner spawner = {
		.x = x,
		.y = y,
		.type = TYPE_EXPLOSION,
		.image = &explosion
	};
	spawn(spawner, EMPTY);
}

void updateExplosion(s16 i){
	if(ent[i].ints[0] >= EXPLOSION_LIMIT) killEntity(i);
	ent[i].ints[0]++;
}