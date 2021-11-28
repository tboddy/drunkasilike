// powerups

#include <genesis.h>
#include <resources.h>

#include "main.h"
#include "powerups.h"
#include "entities.h"
#include "explosion.h"
#include "stage.h"


// spawn

void spawnPowerup(fix16 x, fix16 y){
	struct spawner spawner = {
		.x = x,
		.y = y,
		.image = &powerupPoint,
		.type = TYPE_POWERUP,
		.offX = 8,
		.offY = 8,
		.angle = 512,
		.speed = FIX16(0.5)
	};
	spawner.fixes[0] = spawner.y;
	spawner.ints[1] = random() % 2 < 1 ? 8 : -8;
	spawn(spawner, EMPTY);
}

// spawnPowerup(fix16Add(FIX16(GAME_W), FIX16(8)), fix16Add(FIX16(32), FIX16(random() % (GAME_H - 64))));


// update

void updatePowerup(s16 i){
		ent[i].pos.y = fix16Sub(ent[i].fixes[0], fix16Mul(cosFix16(ent[i].ints[0]), FIX16(32)));
		ent[i].ints[0] += ent[i].ints[1];
	if(ent[i].pos.x < POWERUP_X_LIMIT){
		killEntity(i);
	}
	ent[i].clock++;
}