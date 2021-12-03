 // if not solid_area(a.x + a.dx, a.y, a.w, a.h) then
 //  a.x += a.dx
 // else   
 //  -- otherwise bounce
 //  a.dx *= -a.bounce
 //  sfx(2)
 // end

 // -- ditto for y

 // if not solid_area(a.x, a.y + a.dy, a.w, a.h) then
 //  a.y += a.dy
 // else
 //  a.dy *= -a.bounce
 //  sfx(2)
 // end

// enemies

#include <genesis.h>
#include <resources.h>

#include "main.h"
#include "enemies.h"
#include "player.h"
#include "bullet.h"
#include "stage.h"
#include "chrome.h"


// spawn

void setEnemyPos(s16 i){
	eX = random() % (GAME_W / 16 - 2);
	eY = random() % (GAME_H / 16 - 2);
	if(mapCheck[eX][eY] || (eX > 5 && eY < 9 && eY > 4 && eY < 8)){
		setEnemyPos(i);
	} else {
		mapCheck[eX][eY] = TRUE;
		enemies[i].pos.x = fix16Add(FIX16(eX * 16 + 16), enemies[i].off.x);
		enemies[i].pos.y = fix16Add(FIX16(eY * 16 + 16), enemies[i].off.y);
		enemies[i].last.x = enemies[i].pos.x;
		enemies[i].last.y = enemies[i].pos.y;
	}
}

void spawnEnemy(struct enemySpawner spawner, void(*updater), void(*suicide)){
	s16 i = -1;
	for(s16 j = 0; j < ENEMY_COUNT; j++) if(!enemies[j].active && i == -1) i = j;
	if(i > -1){
		enemies[i].active = TRUE;
		enemies[i].off.x = FIX16(spawner.offX);
		enemies[i].off.y = FIX16(spawner.offY);
		if(spawner.boss){
			enemies[i].pos.x = FIX16(spawner.x);
			enemies[i].pos.y = FIX16(spawner.y);
		} else setEnemyPos(i);
		enemies[i].dist = fix16ToFix32(enemies[i].off.x);
		enemies[i].flipped = FALSE;
		enemies[i].flipClock = 0;
		enemies[i].electrode = spawner.electrode;
		enemies[i].drink = spawner.drink;
		enemies[i].hulk = spawner.hulk;
		enemies[i].boss = spawner.boss;
		enemies[i].speed = spawner.speed;
		enemies[i].angle = spawner.angle;
		enemies[i].health = spawner.health ? spawner.health : 1;
		if(spawner.vel.x && spawner.vel.y){
			enemies[i].vel.x = spawner.vel.x;
			enemies[i].vel.y = spawner.vel.y;
		} else {
			enemies[i].vel.x = fix16Mul(cosFix16(spawner.angle), spawner.speed);
			enemies[i].vel.y = fix16Mul(sinFix16(spawner.angle), spawner.speed);
		}
		for(s16 j = 0; j < COUNT_INT; j++){
			enemies[i].bools[j] = spawner.bools[j];
			enemies[i].ints[j] = spawner.ints[j];
			enemies[i].fixes[j] = spawner.fixes[j];
		}
		enemies[i].updater = updater;
		enemies[i].suicide = suicide;
		enemies[i].clock = 0;
		enemies[i].image = SPR_addSprite(spawner.image,
			fix16ToInt(fix16Sub(enemies[i].pos.x, enemies[i].off.x)),
			fix16ToInt(fix16Sub(enemies[i].pos.y, enemies[i].off.y)),
			TILE_ATTR(PAL1, 0, 0, 0));
		SPR_setAnim(enemies[i].image, spawner.anim ? spawner.anim : 0);
		SPR_setFrame(enemies[i].image, spawner.frame ? spawner.frame : 0);
		if(!enemies[i].boss) SPR_setHFlip(enemies[i].image, random() % 2);
	}
}

void spawnSuicideBullet(s16 i){
	struct bulletSpawner spawner = {
		.x = enemies[i].pos.x,
		.y = enemies[i].pos.y,
		.image = &smallPinkBullet,
		.speed = FIX16(2.5),
		.angle = 256
	};
	spawner.vel = hone(enemies[i].pos, player.pos, spawner.speed, 4);
	spawnBullet(spawner, EMPTY);
}


// update

void collideWall(s16 i){
	enemyDist = getApproximatedDistance(
		fix16ToFix32(fix16Sub(player.pos.x, enemies[i].pos.x)),
		fix16ToFix32(fix16Sub(player.pos.y, enemies[i].pos.y)));
	if(enemyDist <= fix32Add(player.wallDist, enemies[i].dist)){
		// from left or right
		if(player.pos.y > fix16Sub(enemies[i].pos.y, enemies[i].off.y) && player.pos.y < fix16Add(enemies[i].pos.y, enemies[i].off.y)){
			// from left
			if(player.angle == 0 || player.angle == 128 || player.angle == 896)
				player.pos.x = fix16Sub(fix16Sub(enemies[i].pos.x, enemies[i].off.x), fix32ToFix16(player.wallDist2));
			// from right
			else if(player.angle == 512 || player.angle == 384 || player.angle == 640)
				player.pos.x = fix16Add(fix16Add(enemies[i].pos.x, enemies[i].off.x), fix32ToFix16(player.wallDist2));
		}
		// from bottom or top
		else if(player.pos.x > fix16Sub(enemies[i].pos.x, enemies[i].off.x) && player.pos.x < fix16Add(enemies[i].pos.x, enemies[i].off.x)){
			// from bottom
			if(player.angle == 768 || player.angle == 640 || player.angle == 896)
					player.pos.y = fix16Add(fix16Add(enemies[i].pos.y, enemies[i].off.y), fix32ToFix16(player.wallDist2));
			// from top
			else if(player.angle == 256 || player.angle == 128 || player.angle == 384)
				player.pos.y = fix16Sub(fix16Sub(enemies[i].pos.y, enemies[i].off.y), fix32ToFix16(player.wallDist2));
		}
	}
}

void getDrink(s16 i){
	currentScore += happyHour ? 10000 : 5000;
	// spawnLabel(enemies[i].pos.x, enemies[i].pos.y);
}

void collideEnemy(s16 i){
	enemyDist = getApproximatedDistance(
		fix16ToFix32(fix16Sub(player.pos.x, enemies[i].pos.x)),
		fix16ToFix32(fix16Sub(player.pos.y, enemies[i].pos.y)));
	if(enemyDist <= fix32Add(player.dist, enemies[i].dist)){
		if(enemies[i].drink){
			getDrink(i);
		} else {
			spawnExplosion(player.pos.x, player.pos.y, 1);
			XGM_startPlayPCM(random() % 2 < 1 ? SFX_EXPLOSION_1 : SFX_EXPLOSION_2, 1, SOUND_PCM_CH4);
			player.invincible = TRUE;
			player.invincibleClock = INVINCIBLE_LIMIT;
			player.lives--;
			if(player.lives < 0) killPlayer();
		}
		if(!enemies[i].hulk && !enemies[i].boss){
			enemies[i].health -= 5;
			if(enemies[i].health <= 0){
				enemies[i].suicide(i);
				killEnemy(i);
			}
		}
	}
}

void updateEnemy(s16 i){
	if(enemies[i].pos.x < fix16Sub(0, enemies[i].off.x) ||
		enemies[i].pos.x > fix16Add(FIX16(GAME_W), enemies[i].off.x) ||
		enemies[i].pos.y < fix16Sub(0, enemies[i].off.y) ||
		enemies[i].pos.y > fix16Add(FIX16(GAME_H), enemies[i].off.y)){
		killEnemy(i);
	} else {
		if(enemies[i].electrode){
			enemies[i].updater(i);
			collideWall(i);
		} else {
			enemies[i].pos.x = fix16Add(enemies[i].pos.x, enemies[i].vel.x);
			enemies[i].pos.y = fix16Add(enemies[i].pos.y, enemies[i].vel.y);
			enemies[i].updater(i);
			if(enemies[i].boss) bossHealth = enemies[i].health;
			enemies[i].last.x = enemies[i].pos.x;
			enemies[i].last.y = enemies[i].pos.y;
		}
		if(!player.invincible || enemies[i].drink) collideEnemy(i);
		enemies[i].clock++;
		SPR_setPosition(enemies[i].image,
			fix16ToInt(fix16Sub(enemies[i].pos.x, enemies[i].off.x)),
			fix16ToInt(fix16Sub(enemies[i].pos.y, enemies[i].off.y)));
	}
}

void updateEnemyVel(s16 i){
	enemies[i].vel.x = fix16Mul(cosFix16(enemies[i].angle), enemies[i].speed);
	enemies[i].vel.y = fix16Mul(sinFix16(enemies[i].angle), enemies[i].speed);
}

void killEnemy(s16 i){
	enemies[i].active = FALSE;
	SPR_releaseSprite(enemies[i].image);
}

void flickerEnemy(s16 i){
	SPR_setVisibility(enemies[i].image, waveClock % 20 >= 10 ? HIDDEN : VISIBLE);
}

void flipEnemy(s16 i){
	if(enemies[i].flipped){
		enemies[i].flipClock++;
		if(enemies[i].flipClock >= 30) enemies[i].flipped = FALSE;
	} else if(enemies[i].pos.x < enemies[i].off.x ||
		enemies[i].pos.x > fix16Sub(FIX16(GAME_W), enemies[i].off.x) ||
		enemies[i].pos.y < enemies[i].off.y ||
		enemies[i].pos.y > fix16Sub(FIX16(GAME_H), enemies[i].off.y)){
		enemies[i].angle = 512 - enemies[i].angle;
		if(enemies[i].pos.y < enemies[i].off.y || enemies[i].pos.y > fix16Sub(FIX16(GAME_H), enemies[i].off.y)) enemies[i].angle -= 512;
		updateEnemyVel(i);
		enemies[i].flipClock = 0;
		enemies[i].flipped = TRUE;
	}
}


// loop

void updateEnemies(){
	if(!gameOver && !paused) {
		if(waveClock >= 90){
			if(waveClock == 90) for(s16 i = 0; i < ENEMY_COUNT; i++) if(enemies[i].active) SPR_setVisibility(enemies[i].image, VISIBLE);
			if(clock % 2 == 1) for(s16 i = 0; i < ENEMY_COUNT; i++) if(enemies[i].active){
				updateEnemy(i);
			}
		} else {
			for(s16 i = 0; i < ENEMY_COUNT; i++) if(enemies[i].active) flickerEnemy(i);
		}
	}
}