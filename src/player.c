// player

#include <genesis.h>
#include <resources.h>

#include "main.h"
#include "controls.h"
#include "bullet.h"
#include "enemies.h"
#include "player.h"
#include "chrome.h"
#include "stage.h"


// spawn

void spawnPlayer(){
	player.pos.x = PLAYER_INIT_X;
	player.pos.y = PLAYER_INIT_Y;
	player.off.x = FIX16(8);
	player.off.y = FIX16(8);
	player.dist = FIX32(1);
	player.wallDist = FIX32(6);
	player.wallDist2 = FIX32(8);
	player.image = SPR_addSprite(
		&suika,
		fix16ToInt(fix16Sub(player.pos.x, player.off.x)) + GAME_X,
		fix16ToInt(fix16Sub(player.pos.y, player.off.y)) + GAME_Y,
		TILE_ATTR(PAL1, 0, 0, 0));
	SPR_setVisibility(player.image, HIDDEN);
}


// move

void collidePlayer(){
	if(player.pos.x < PLAYER_LIMIT_X) player.pos.x = PLAYER_LIMIT_X;
	else if(player.pos.x > PLAYER_LIMIT_W) player.pos.x = PLAYER_LIMIT_W;
	if(player.pos.y < PLAYER_LIMIT_Y) player.pos.y = PLAYER_LIMIT_Y;
	else if(player.pos.y > PLAYER_LIMIT_Z) player.pos.y = PLAYER_LIMIT_Z;
}

void movePlayer(){
	if((ctrl.left || ctrl.right || ctrl.up || ctrl.down) && enemyCount > 0 && waveClock >= PLAYER_START_TIME){
		if(ctrl.left || ctrl.right){
			if(ctrl.up){
				player.angle = ctrl.left ? 640 : 896;
			} else if(ctrl.down){
				player.angle = ctrl.left ? 384 : 128;
			} else {
				player.angle = ctrl.left ? 512 : 0;
			}
		} else if(ctrl.up){
			player.angle = 768;
		} else if(ctrl.down){
			player.angle = 256;
		}
		player.pos.x = fix16Add(player.pos.x, fix16Mul(cosFix16(player.angle), PLAYER_SPEED));
		player.pos.y = fix16Add(player.pos.y, fix16Mul(sinFix16(player.angle), PLAYER_SPEED));
		collidePlayer();
		if(player.angle != player.shotAngle && !ctrl.b){
			player.shotAngle = player.angle;
			switch(player.angle){
				case 0: SPR_setAnim(player.image, 3); break;
				case 256: SPR_setAnim(player.image, 0); break;
				case 768: SPR_setAnim(player.image, 1); break;
				case 512: SPR_setAnim(player.image, 2); break;
			}
		}
	}
	player.moveClock++;
	if(player.moveClock >= CLOCK_LIMIT) player.moveClock = 0;
}

void updateInvincible(){
	if(waveClock >= 0){
		if(player.invincibleClock % 20 == 0) {
			SPR_setVisibility(player.image, HIDDEN);
		} else if(player.invincibleClock % 20 == 10){
			SPR_setVisibility(player.image, VISIBLE);
		}
	}
	player.invincibleClock--;
	if(player.invincibleClock <= 0){
		player.invincible = FALSE;
	}
}

void killPlayer(){
	// player.lives = 3;
	gameOver = TRUE;
	transitionX = 0;
	transitionY = 0;
	for(s16 i = 0; i < ENEMY_COUNT; i++) if(enemies[i].active) killEnemy(i);
	for(s16 i = 0; i < BULLET_COUNT; i++) if(bullets[i].active) killBullet(i);
}


// shoot

void spawnPlayerBullet(){
	struct bulletSpawner spawner = {
		.x = player.pos.x,
		.y = player.pos.y,
		.image = &smallWhiteBullet,
		.speed = FIX16(16),
		.angle = player.shotAngle,
		.player = TRUE
	};
	spawnBullet(spawner, EMPTY);
	XGM_startPlayPCM(SFX_PLAYER_SHOT, 0, SOUND_PCM_CH4);
}

void updatePlayerShot(){
	if(player.shotClock >= SHOT_INTERVAL && ctrl.a) player.shotClock = 0;
	if(player.shotClock == 0 && enemyCount > 0 && waveClock >= PLAYER_START_TIME) spawnPlayerBullet();
	player.shotClock++;
	if(player.shotClock >= CLOCK_LIMIT) player.shotClock = SHOT_INTERVAL;
}


// loop

void updatePlayer(){
	if(gameOver){
		SPR_setVisibility(player.image, HIDDEN);
		if((ctrl.a || ctrl.b || ctrl.c || ctrl.start) && gameOverClock >= 60) resetGame();
	} else if(!paused && !gameOver) {
		movePlayer();
		updatePlayerShot();
		SPR_setPosition(player.image, fix16ToInt(fix16Sub(player.pos.x, player.off.x)) + GAME_X, fix16ToInt(fix16Sub(player.pos.y, player.off.y)) + GAME_Y);
		if(player.invincible) updateInvincible();
		player.clock++;
		if(player.clock >= CLOCK_LIMIT) player.clock = 0;
	}
}

void loadPlayer(){
	player.shotClock = SHOT_INTERVAL;
	spawnPlayer();
}