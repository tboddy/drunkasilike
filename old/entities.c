// entities

#include <genesis.h>
#include <resources.h>

#include "main.h"
#include "entities.h"
#include "enemies.h"
#include "bullet.h"
#include "player.h"
#include "explosion.h"
#include "powerups.h"


// spawn

void spawn(struct spawner spawner, void(*updater)){
	s16 i = -1;
	for(s16 j = 0; j < ENTITY_COUNT; j++) if(!ent[j].active && i == -1){
		i = j;
		break;
	}
	ent[i].active = TRUE;
	ent[i].pos.x = spawner.x;
	ent[i].pos.y = spawner.y;
	ent[i].type = spawner.type;
	ent[i].subType = spawner.subType;
	ent[i].health = spawner.health;
	ent[i].clock = 0;
	ent[i].updater = updater;
	if(spawner.vel.x || spawner.vel.y){
		ent[i].vel.x = spawner.vel.x;
		ent[i].vel.y = spawner.vel.y;
	} else {
		ent[i].angle = spawner.angle;
		ent[i].speed = spawner.speed;
		UPDATE_VEL(i);
	}
	for(s16 j = 0; j < COUNT_INT; j++){
		ent[i].bools[j] = spawner.bools[j];
		ent[i].ints[j] = spawner.ints[j];
		ent[i].fixes[j] = spawner.fixes[j];
		ent[i].vectors[j].x = spawner.vectors[j].x;
		ent[i].vectors[j].y = spawner.vectors[j].y;
	}
	for(s16 j = 0; j < COLLIDER_INT; j++){
		ent[i].colliders[j] = FALSE;
		ent[i].colliderV[j].x = -1;
		ent[i].colliderV[j].y = -1;
	}
	ent[i].offX = spawner.offX;
	ent[i].offY = spawner.offY;
	switch(ent[i].type){
		case TYPE_EXPLOSION:
			ent[i].offX = 12;
			ent[i].offY = 12;
			break;
	}
	ent[i].image = SPR_addSprite(spawner.image, fix16ToInt(ent[i].pos.x) - ent[i].offX, fix16ToInt(ent[i].pos.y) - ent[i].offY, S_ATTR);
	if(spawner.anim) SPR_setAnim(ent[i].image, spawner.anim);
}


// helpers

Vect2D_f16 hone(Vect2D_f16 pos, Vect2D_f16 target, fix16 speed, s16 lerp){
	hPos.x = target.x;
	hPos.y = target.y;
	if(lerp > 0){
		hPos.x = fix16Add(fix16Sub(hPos.x, FIX16(lerp)), FIX16(random() % (lerp * 2)));
		hPos.y = fix16Add(fix16Sub(hPos.y, FIX16(lerp)), FIX16(random() % (lerp * 2)));
	}
	honeSpeed = fix32ToFix16(getApproximatedDistance(
		fix32Sub(fix16ToFix32(pos.x), fix16ToFix32(hPos.x)),
		fix32Sub(fix16ToFix32(pos.y), fix16ToFix32(hPos.y))));
	hPos.x = fix16Mul(fix16Div(fix16Sub(hPos.x, pos.x), honeSpeed), speed);
	hPos.y = fix16Mul(fix16Div(fix16Sub(hPos.y, pos.y), honeSpeed), speed);
	return hPos;
}


// loop

void killEntity(s16 i){
	ent[i].active = FALSE;
	SPR_releaseSprite(ent[i].image);
}

void updateEntity(s16 i){
	switch(ent[i].type){
		case TYPE_PLAYER: updatePlayer(i); break;
		case TYPE_ENEMY: updateEnemy(i); break;
		case TYPE_BULLET: updateBullet(i); break;
		case TYPE_EXPLOSION: updateExplosion(i); break;
		case TYPE_POWERUP: updatePowerup(i); break;
	}
	ent[i].updater(i);
	ent[i].pos.x = fix16Add(ent[i].pos.x, ent[i].vel.x);
	ent[i].pos.y = fix16Add(ent[i].pos.y, ent[i].vel.y);
}

void drawEntity(s16 i){
	SPR_setPosition(ent[i].image, fix16ToInt(ent[i].pos.x) - ent[i].offX, fix16ToInt(ent[i].pos.y) - ent[i].offY);
}


// main loop

void loadEntities(){
	loadPlayer();
}

void updateEntities(){
	for(s16 i = 0; i < ENTITY_COUNT; i++) if(ent[i].active){
		updateEntity(i);
		drawEntity(i);
	}
	for(s16 i = 0; i < ENTITY_COUNT; i++) if(ent[i].active && ent[i].type == TYPE_BULLET) collideBullet(i);
	updateEnemies();
}