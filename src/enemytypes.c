// enemy types

#include <genesis.h>
#include <resources.h>

#include "main.h"
#include "enemies.h"
#include "bullet.h"
#include "player.h"
#include "chrome.h"
#include "stage.h"
#include "explosion.h"
#include "enemytypes.h"


// types

void spawnGrunt(){
	struct enemySpawner spawner = {
		.offX = 8,
		.offY = 8,
		.image = &beer,
		.health = 3,
		.anim = random() % 2,
		.angle = random() % 1024,
		.speed = FIX16(0.5)
	};
	spawner.ints[0] = random() % 60;
	void updater(s16 i){
		if(enemies[i].clock % 60 == enemies[i].ints[0]) enemies[i].vel = hone(enemies[i].pos, player.pos, enemies[i].speed, 8);

		if(enemies[i].health <= 2 && !enemies[i].bools[0]){
			SPR_setFrame(enemies[i].image, 1);
			enemies[i].bools[0] = TRUE;
		} else if(enemies[i].health <= 1 && !enemies[i].bools[1]){
			SPR_setFrame(enemies[i].image, 2);
			enemies[i].bools[1] = TRUE;
		}

	};
	void suicide(s16 i){
		currentScore += 750;
		enemyCount--;
	}
	spawnEnemy(spawner, updater, suicide);
	enemyCount++;
}

void spawnHulk(){
	struct enemySpawner spawner = {
		.offX = 8,
		.offY = 8,
		.image = &pickle,
		.hulk = TRUE,
		.angle = random() % 4 * 256,
		.speed = FIX16(0.25)
	};
	spawner.ints[0] = random() % 60;
	void updater(s16 i){
		if(enemies[i].pos.y < enemies[i].off.y || enemies[i].pos.x < enemies[i].off.x ||
			enemies[i].pos.y > fix16Sub(FIX16(GAME_H), enemies[i].off.y) || enemies[i].pos.x > fix16Sub(FIX16(GAME_W), enemies[i].off.x)){
			enemies[i].angle += 512;
			updateEnemyVel(i);
		}
		if(enemies[i].clock % 60 == enemies[i].ints[0]){
			enemies[i].angle += random() % 2 < 1 ? -256 : 256;
			if(enemies[i].angle > 1024) enemies[i].angle -= 1024;
			else if(enemies[i].angle < -1024) enemies[i].angle += 1024;
			updateEnemyVel(i);
			struct bulletSpawner spawner = {
				.x = enemies[i].pos.x,
				.y = enemies[i].pos.y,
				.speed = FIX16(2),
				.angle = random() % 1024,
				.image = &smallYellowBullet
			};
			if(fastBullets) spawner.speed = fix16Add(spawner.speed, FAST_BULLET_MOD);
			for(s16 j = 0; j < 3; j++){
				spawnBullet(spawner, EMPTY);
				spawner.angle += 341;
			}
			XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
		}
	}
	spawnEnemy(spawner, updater, EMPTY);
}

void spawnElectrode(){
	struct enemySpawner spawner = {
		.offX = 8,
		.offY = 8,
		.health = 15,
		.electrode = TRUE,
		.image = &lantern,
		.frame = random() % 4
	};
	spawner.ints[0] = random() % 60;
	spawner.ints[1] = 0;
	void updater(s16 i){
		if(enemies[i].health <= 10 && !enemies[i].bools[0]){
			SPR_setAnim(enemies[i].image, 1);
			enemies[i].bools[0] = TRUE;
		} else if(enemies[i].health <= 5 && !enemies[i].bools[1]){
			SPR_setAnim(enemies[i].image, 2);
			enemies[i].bools[1] = TRUE;
		}
	}
	spawnEnemy(spawner, updater, EMPTY);
}

void spawnEnforcer(){
	struct enemySpawner spawner = {
		.offX = 8,
		.offY = 8,
		.image = &wine,
		.health = 6,
		.anim = random() % 2,
		.angle = random() % 4 * 256,
		.speed = FIX16(0.5)
	};
	spawner.ints[0] = random() % 60;
	void updater(s16 i){
		if(enemies[i].pos.y < enemies[i].off.y || enemies[i].pos.x < enemies[i].off.x ||
			enemies[i].pos.y > fix16Sub(FIX16(GAME_H), enemies[i].off.y) || enemies[i].pos.x > fix16Sub(FIX16(GAME_W), enemies[i].off.x)){
			enemies[i].angle += 512;
			updateEnemyVel(i);
		}
		if(enemies[i].clock % 60 == enemies[i].ints[0]){
			enemies[i].angle += random() % 2 < 1 ? -256 : 256;
			if(enemies[i].angle > 1024) enemies[i].angle -= 1024;
			else if(enemies[i].angle < -1024) enemies[i].angle += 1024;
			updateEnemyVel(i);
			struct bulletSpawner spawner = {
				.x = enemies[i].pos.x,
				.y = enemies[i].pos.y,
				.speed = FIX16(2),
				.image = &bigGreenBullet,
				.big = TRUE
			};
			if(fastBullets) spawner.speed = fix16Add(spawner.speed, FAST_BULLET_MOD);
			for(s16 j = 0; j < 2; j++){
				if(j > 0) spawner.image = &smallGreenBullet;
				spawner.vel = hone(enemies[i].pos, player.pos, spawner.speed, j == 0 ? 16 : 64);
				spawnBullet(spawner, EMPTY);
			}
			XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
		}
		if(enemies[i].health <= 4 && !enemies[i].bools[0]){
			SPR_setFrame(enemies[i].image, 1);
			enemies[i].bools[0] = TRUE;
		} else if(enemies[i].health <= 2 && !enemies[i].bools[1]){
			SPR_setFrame(enemies[i].image, 2);
			enemies[i].bools[1] = TRUE;
		}
	}
	void suicide(s16 i){
		currentScore += 2000;
		enemyCount--;
	}
	spawnEnemy(spawner, updater, suicide);
	enemyCount++;
}

void spawnTank(){
	struct enemySpawner spawner = {
		.offX = 8,
		.offY = 8,
		.image = &martini,
		.health = 6,
		.anim = random() % 2,
		.angle = random() % 4 * 256,
		.speed = FIX16(0.25),
		.angle = random() % 1024
	};
	spawner.ints[0] = 30 + random() % 90;
	void updater(s16 i){
		if(enemies[i].clock % 120 == enemies[i].ints[0]){
			struct bulletSpawner spawner = {
				.x = enemies[i].pos.x,
				.y = enemies[i].pos.y,
				.speed = FIX16(2),
				.image = &smallRedBullet,
				.angle = random() % 256
			};
			if(fastBullets) spawner.speed = fix16Add(spawner.speed, FAST_BULLET_MOD);
			void updater(s16 j){
				if(!bullets[j].bools[0] && (bullets[j].pos.x <= bullets[j].off.x ||
					bullets[j].pos.x >= fix16Sub(FIX16(GAME_W), bullets[j].off.x) ||
					bullets[j].pos.y <= bullets[j].off.y ||
					bullets[j].pos.y >= fix16Sub(FIX16(GAME_H), bullets[j].off.y))){
					bullets[j].angle = 512 - bullets[j].angle;
					if(bullets[j].pos.y < bullets[j].off.y || bullets[j].pos.y > fix16Sub(FIX16(GAME_H), bullets[j].off.y)) bullets[j].angle -= 512;
					updateBulletVel(j);
					bullets[j].bools[0] = TRUE;
				}
			}
			for(s16 j = 0; j < 5; j++){
				spawnBullet(spawner, updater);
				spawner.angle += 205;
			}
			XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
		}
		if(enemies[i].health <= 4 && !enemies[i].bools[0]){
			SPR_setFrame(enemies[i].image, 1);
			enemies[i].bools[0] = TRUE;
		} else if(enemies[i].health <= 2 && !enemies[i].bools[1]){
			SPR_setFrame(enemies[i].image, 2);
			enemies[i].bools[1] = TRUE;
		}
		flipEnemy(i);
	}
	void suicide(s16 i){
		currentScore += 5000;
		enemyCount--;
	}
	spawnEnemy(spawner, updater, suicide);
	enemyCount++;
}

void spawnSushi(){
	struct enemySpawner spawner = {
		.offX = 8,
		.offY = 8,
		.drink = TRUE,
		.angle = random() % 1024,
		.speed = FIX16(0.5)
	};
	spawner.frame = random() % 2;
	spawner.image = &sushi1;
	switch(random() % 3){
		case 0: spawner.image = &sushi1; break;
		case 1: spawner.image = &sushi2; break;
		case 2: spawner.image = &sushi3; break;
	}
	void updater(s16 i){
		flipEnemy(i);
	}
	spawnEnemy(spawner, updater, EMPTY);
}

// boss patterns
void patternOne(s16 i){  // random triangles small
	if(enemies[i].clock % 5 == 0){
		struct bulletSpawner spawner = {
			.image = enemies[i].clock % 60 < 30 ? &smallRedBullet : &smallBlueBullet,
			.angle = random() % 1024,
			.speed = FIX16(3)
		};
		enemies[i].fixes[0] = FIX16(40);
		spawner.x = fix16Add(enemies[i].pos.x, fix16Mul(cosFix16(spawner.angle), enemies[i].fixes[0]));
		spawner.y = fix16Add(enemies[i].pos.y, fix16Mul(sinFix16(spawner.angle), enemies[i].fixes[0]));
		for(s16 j = 0; j < 3; j++){
			spawnBullet(spawner, EMPTY);
			spawner.angle += 341;
		}
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
	}
}

void patternTwo(s16 i){ // curvy boys that shoot and ball circle
	if(enemies[i].clock % 90 == 0){
		struct bulletSpawner spawner = {
			.x = enemies[i].pos.x,
			.y = enemies[i].pos.y,
			.image = &bigBlueBullet,
			.big = TRUE,
			.angle = enemies[i].clock % 240 == 0 ? 0 : 64,
			.speed = FIX16(2.5)
		};
		void updater(s16 j){
			if(bullets[j].clock > 0 && bullets[j].clock % 20 == 0){
				bullets[j].angle += bullets[j].clock % 40 == 0 ? -256 : 256;
				updateBulletVel(j);
			}
			if(bullets[j].clock > 0 && bullets[j].clock % 15 == 0 && bullets[j].clock <= 60){
				struct bulletSpawner spawner = {
					.x = bullets[j].pos.x,
					.y = bullets[j].pos.y,
					.image = &smallBlueBullet,
					.angle = 0,
					.speed = 0
				};
				void updater(s16 h){
					if(bullets[h].clock == 30)
						bullets[h].vel = hone(bullets[h].pos, player.pos, FIX16(3), 256);
				}
				spawnBullet(spawner, updater);
				XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
			}
		}
		for(s16 j = 0; j < 6; j++){
			spawnBullet(spawner, updater);
			spawner.angle += 171;
		}
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
	}
	if(enemies[i].clock % 90 == 60){
		struct bulletSpawner spawner = {
			.x = enemies[i].pos.x,
			.y = enemies[i].pos.y,
			.image = &bigRedBullet,
			.big = TRUE,
			.angle = random() % 128,
			.speed = FIX16(3)
		};
		for(s16 j = 0; j < 11; j++){
			spawnBullet(spawner, EMPTY);
			spawner.angle += 93;
		}
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
	}
}

void patternThree(s16 i){ // balls in 4 corners
	if(enemies[i].clock % 60 < 40){
		if(enemies[i].clock % 60 == 0){
			enemies[i].ints[0] = random() % 1024;
			switch(enemies[i].clock % 240){
				case 0:
					enemies[i].fixes[0] = FIX16(32);
					enemies[i].fixes[1] = FIX16(32);
					break;
				case 60:
					enemies[i].fixes[0] = FIX16(GAME_W - 32);
					enemies[i].fixes[1] = FIX16(GAME_H - 32);
					break;
				case 120:
					enemies[i].fixes[0] = FIX16(GAME_W - 32);
					enemies[i].fixes[1] = FIX16(32);
					break;
				case 180:
					enemies[i].fixes[0] = FIX16(32);
					enemies[i].fixes[1] = FIX16(GAME_H - 32);
					break;
			}
		}
		struct bulletSpawner spawner = {
			.x = enemies[i].fixes[0],
			.y = enemies[i].fixes[1],
			.angle = enemies[i].ints[0],
			.speed = FIX16(1.5),
			.big = TRUE
		};
		spawner.bools[0] = enemies[i].clock % 120 < 50;
		spawner.fixes[0] = FIX16(1.5);
		spawner.ints[0] = spawner.bools[0] ? -195 : 324;
		spawner.image = spawner.bools[0] ? &bigBlueBullet : &bigRedBullet;
		void updater(s16 j){
			if(bullets[j].clock > 0 && bullets[j].clock % 10 == 0 && bullets[j].clock < 60){
				bullets[j].speed = fix16Sub(bullets[j].speed, bullets[j].fixes[0]);
				updateBulletVel(j);
			}
		}
		spawnBullet(spawner, updater);
		enemies[i].ints[0] += spawner.ints[0];
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
	}
}

void patternFour(s16 i){ // double ballz
	if(enemies[i].clock % 30 == 0 || enemies[i].clock % 30 == 10){
		struct bulletSpawner spawner = {
			.y = enemies[i].pos.y,
			.angle = random() % 1024,
			.speed = FIX16(2.5),
			.big = TRUE,
			.image = &bigBlueBullet
		};
		spawner.bools[0] = enemies[i].clock % 30 == 0;
		spawner.fixes[0] = FIX16(32);
		spawner.fixes[1] = FIX16(0.25);
		spawner.fixes[2] = FIX16(32);
		spawner.ints[0] = spawner.bools[0] ? 64 : -64;
		spawner.x = fix16Add(enemies[i].pos.x, fix16Mul(cosFix16(spawner.angle), spawner.fixes[2]));
		spawner.y = fix16Add(enemies[i].pos.y, fix16Mul(sinFix16(spawner.angle), spawner.fixes[2]));
		void updater(s16 h){
			if(bullets[h].clock % 15 == 0 && bullets[h].clock > 0 && bullets[h].clock < 90){
				bullets[h].angle += bullets[h].ints[0];
				// bullets[h].speed = fix16Sub(bullets[h].speed, bullets[h].fixes[1]);
				updateBulletVel(h);
			}
		}
		for(s16 j = 0; j < 9; j++){
			spawnBullet(spawner, updater);
			spawner.angle += 114;
		}
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
	}
	else if(enemies[i].clock % 30 > 15 && enemies[i].clock % 5 == 0){
		struct bulletSpawner spawner = {
			.x = enemies[i].pos.x,
			.y = enemies[i].pos.y,
			.angle = 256,
			.speed = FIX16(2),
			.image = &smallRedBullet
		};
		spawner.vel = hone(enemies[i].pos, player.pos, spawner.speed, 64);
		spawnBullet(spawner, EMPTY);
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
	}
}

void patternFive(s16 i){ // big swash
	if(enemies[i].clock % 2 == 0 && enemies[i].clock % 60 <= 50){
		if(enemies[i].clock % 60 == 0){
			enemies[i].ints[0] = random() % -32 - 32;
			enemies[i].ints[1] = enemies[i].clock % 120 < 60 ? 32 : -32;
			enemies[i].fixes[0] = FIX16(32);
			if(enemies[i].clock % 120 >= 60) enemies[i].ints[0] += 512;
		}
		struct bulletSpawner spawner = {
			.x = fix16Add(enemies[i].pos.x, fix16Mul(cosFix16(enemies[i].ints[0]), enemies[i].fixes[0])),
			.y = fix16Add(enemies[i].pos.y, fix16Mul(sinFix16(enemies[i].ints[0]), enemies[i].fixes[0])),
			.angle = enemies[i].ints[0],
			.speed = FIX16(0),
			.big = TRUE
		};
		spawner.bools[0] = enemies[i].clock % 120 < 60;
		spawner.fixes[0] = FIX16(0.5);
		spawner.ints[0] = spawner.bools[0] ? 20 : -20;
		spawner.image = spawner.bools[0] ? &bigBlueBullet : &bigRedBullet;
		void updater(s16 j){
			if(bullets[j].clock % 5 == 0 && bullets[j].clock > 10){
				bullets[j].speed = fix16Add(bullets[j].speed, bullets[j].fixes[0]);
				bullets[j].angle += bullets[j].ints[0];
				updateBulletVel(j);
			}
		}
		spawnBullet(spawner, updater);
		enemies[i].ints[0] += enemies[i].ints[1];
		enemies[i].ints[1] += spawner.bools[0] ? 2 : -2;
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
	}
	if(enemies[i].clock % 30 == 0 && enemies[i].clock > 0){
		struct bulletSpawner spawner = {
			.x = enemies[i].pos.x,
			.y = enemies[i].pos.y,
			.angle = enemies[i].ints[6],
			.speed = FIX16(2.5)
		};
		spawner.bools[0] = enemies[i].clock % 120 < 60;
		spawner.image = spawner.bools[0] ? &smallRedBullet : &smallBlueBullet;
		for(s16 j = 0; j < 5; j++){
			spawnBullet(spawner, EMPTY);
			spawner.angle += 205;
		}
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
		enemies[i].ints[6] += 79;
		if(enemies[i].ints[6] > 1024) enemies[i].ints[6] -= 1024;
	}
}

void patternSix(s16 i){ // small + balls
	if(enemies[i].clock % 20 == 0){
		struct bulletSpawner spawner = {
			.speed = FIX16(5),
			.image = &bigRedBullet,
			.big = TRUE,
			.angle = random() % 1024
		};
		spawner.x = fix16Add(enemies[i].pos.x, fix16Mul(cosFix16(spawner.angle), FIX16(32)));
		spawner.y = fix16Add(enemies[i].pos.y, fix16Mul(sinFix16(spawner.angle), FIX16(32)));
		spawner.ints[0] = enemies[i].clock % 60 < 30 ? -24 : 24;
		void updater(s16 j){
			if(bullets[j].clock == 5){
				bullets[j].angle += 512;
				bullets[j].speed = FIX16(0);
				updateBulletVel(j);
			} else if(bullets[j].clock == 15){
				bullets[j].angle += 512;
				bullets[j].speed = FIX16(2.5);
				updateBulletVel(j);
			} else if(bullets[j].clock > 15 && bullets[j].clock % 5 == 0 && bullets[j].clock < 60){
				bullets[j].angle += bullets[j].ints[0];
				updateBulletVel(j);
			}
		}
		for(s16 j = 0; j < 8; j++){
			spawner.bools[0] = i % 2 == 0;
			spawnBullet(spawner, updater);
			spawner.angle += 128;
		}
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
	}
	if(enemies[i].clock % 10 == 5 && enemies[i].clock % 30 >= 15){
		struct bulletSpawner spawner = {
			.x = enemies[i].pos.x,
			.y = enemies[i].pos.y,
			.speed = FIX16(2.5),
			.image = &bigBlueBullet,
			.big = TRUE,
			.angle = 256
		};
		spawner.vel = hone(enemies[i].pos, player.pos, spawner.speed, 8);
		spawnBullet(spawner, EMPTY);
		spawner.speed = FIX16(2);
		spawner.big = FALSE;
		spawner.image = &smallBlueBullet;
		spawner.vel = hone(enemies[i].pos, player.pos, spawner.speed, 64);
		spawnBullet(spawner, EMPTY);
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
	}
}

void patternSeven(s16 i){ // ballz to edges
	if(enemies[i].clock % 90 == 0){
		struct bulletSpawner spawner = {
			.x = enemies[i].pos.x,
			.y = enemies[i].pos.y,
			.speed = FIX16(3),
			.image = &bigRedBullet,
			.big = TRUE,
			.angle = random() % 1024
		};
		spawner.fixes[0] = FIX16(GAME_H);
		spawner.fixes[1] = FIX16(GAME_W);
		void updater(s16 j){
			if(!bullets[j].bools[0] && (bullets[j].pos.x <= bullets[j].off.x ||
				bullets[j].pos.x >= fix16Sub(FIX16(GAME_W), bullets[j].off.x) ||
				bullets[j].pos.y <= bullets[j].off.y ||
				bullets[j].pos.y >= fix16Sub(FIX16(GAME_H), bullets[j].off.y))){
				bullets[j].angle = 512 - bullets[j].angle;
				if(bullets[j].pos.y < bullets[j].off.y || bullets[j].pos.y > fix16Sub(FIX16(GAME_H), bullets[j].off.y)) bullets[j].angle -= 512;
				updateBulletVel(j);
				bullets[j].clock = -1;
				bullets[j].bools[0] = TRUE;
			} else if(bullets[j].bools[0] && bullets[j].clock % 45 == 0){
				struct bulletSpawner spawner = {
					.x = bullets[j].pos.x,
					.y = bullets[j].pos.y,
					.speed = FIX16(2.5),
					.image = &smallBlueBullet,
					.angle = random() % 1024
				};
				for(s16 h = 0; h < 4; h++){
					spawnBullet(spawner, EMPTY);
					spawner.angle += 256;
				}
				XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
			}
		}
		for(s16 i = 0; i < 7; i++){
			spawnBullet(spawner, updater);
			spawner.angle += 146;
		}
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
	}
}

void patternEight(s16 i){ // corner + burst
	if(enemies[i].clock % 30 == 0){
		struct bulletSpawner spawner = {
			.x = 0,
			.y = 0,
			.speed = FIX16(2.5),
			.image = &bigRedBullet,
			.big = TRUE,
			.angle = random() % 32 + 16
		};
		if(enemies[i].clock % 120 == 30){
			spawner.x = FIX16(GAME_W);
			spawner.angle += 256;
		} else if(enemies[i].clock % 120 == 60){
			spawner.x = FIX16(GAME_W);
			spawner.y = FIX16(GAME_H);
			spawner.angle += 512;
		} else if(enemies[i].clock % 120 == 90){
			spawner.y = FIX16(GAME_H);
			spawner.angle += 768;
		}
		for(i = 0; i < 4; i++){
			spawnBullet(spawner, EMPTY);
			spawner.angle += 64;
		}
		spawner.image = &smallRedBullet;
		spawner.big = FALSE;
		stageVectors[0].x = spawner.x;
		stageVectors[0].y = spawner.y;
		spawner.vel = hone(stageVectors[0], player.pos, spawner.speed, 48);
		spawnBullet(spawner, EMPTY);
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
	}
	if(enemies[i].clock % 60 == 0) enemies[i].ints[0] = random() * 32 + 32;
	else if(enemies[i].clock % 60 >= 50){
		struct bulletSpawner spawner = {
			.x = enemies[i].pos.x,
			.y = enemies[i].pos.y,
			.speed = FIX16(2.5),
			.image = &bigBlueBullet,
			.big = TRUE,
			.angle = enemies[i].ints[0]
		};
		spawnBullet(spawner, EMPTY);
		enemies[i].ints[0] += 90;
		if(enemies[i].ints[0] > 1024) enemies[i].ints[0] -= 1024;
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
	}
}

void patternNine(s16 i){ // triad
	if(enemies[i].clock % 3 == 0){
		struct bulletSpawner spawner = {
			.speed = FIX16(4),
			.image = &bigBlueBullet,
			.big = TRUE,
			.angle = enemies[i].ints[0]
		};
		spawner.fixes[0] = FIX16(32);
		for(s16 j = 0; j < 3; j++){
			spawner.x = fix16Add(enemies[i].pos.x, fix16Mul(cosFix16(spawner.angle), spawner.fixes[0]));
			spawner.y = fix16Add(enemies[i].pos.y, fix16Mul(sinFix16(spawner.angle), spawner.fixes[0]));
			spawnBullet(spawner, EMPTY);
			spawner.angle += 341;
		}
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
		enemies[i].ints[0] += 90;
		if(enemies[i].ints[0] > 1024) enemies[i].ints[0] -= 1024;
	} else if(enemies[i].clock % 3 == 2){
		struct bulletSpawner spawner = {
			.x = enemies[i].pos.x,
			.y = enemies[i].pos.y,
			.speed = FIX16(3.5),
			.image = &smallRedBullet,
			.angle = 256
		};
		stageVectors[0].x = spawner.x;
		stageVectors[0].y = spawner.y;
		spawner.vel = hone(stageVectors[0], player.pos, spawner.speed, 96);
		spawnBullet(spawner, EMPTY);
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
	}
}

void patternTen(s16 i){ // small + bubbles
	if(enemies[i].clock % 6 == 0){
		struct bulletSpawner spawner = {
			.speed = FIX16(6),
			.image = &bigBlueBullet,
			.big = TRUE,
			.angle = 768 - 16 + random() % 32,
			.y = FIX16(GAME_H),
			.x = enemies[i].clock % 12 == 0 ? FIX16(GAME_W / 4) : FIX16(GAME_W / 4 * 3)
		};
		spawner.fixes[0] = FIX16(0.5);
		void updater(s16 j){
			if(bullets[j].clock % 5 == 0 && bullets[j].clock > 0){
				bullets[j].speed = fix16Sub(bullets[j].speed, bullets[j].fixes[0]);
				updateBulletVel(j);
			}
			if(bullets[j].speed <= 0 && !bullets[j].bools[0]){
				bullets[j].angle += -64 + random() % 128;
				updateBulletVel(j);
				bullets[j].bools[0] = TRUE;
				struct bulletSpawner spawner = {
					.speed = FIX16(4),
					.image = &smallRedBullet,
					.big = FALSE,
					.angle = random() % 128,
					.y = bullets[j].pos.y,
					.x = bullets[j].pos.x
				};
				for(s16 h = 0; h < 4; h++){
					spawnBullet(spawner, EMPTY);
					spawner.angle += 128;
				}
				XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
			}
		}
		spawnBullet(spawner, updater);
		XGM_startPlayPCM(SFX_BULLET_1, 0, SOUND_PCM_CH3);
	}
}

void spawnSake(){
	struct enemySpawner spawner = {
		.offX = 32,
		.offY = 32,
		.image = &sake,
		.angle = 256,
		.speed = FIX16(2),
		.boss = TRUE
	};
	switch(currentBossNum){
		case 0: spawner.health = 75; break;
		case 1: spawner.health = 75; break;
		case 2: spawner.health = 100; break;
		case 3: spawner.health = 100; break;
		case 4: spawner.health = 100; break;
		case 5: spawner.health = 125; break;
		case 6: spawner.health = 125; break;
		case 7: spawner.health = 175; break;
		case 8: spawner.health = 175; break;
		case 9: spawner.health = 150; break;
	}
	spawner.ints[8] = 64 + random() % 384;
	switch(random() % 4){
		case 0:
			spawner.x = GAME_W / 2;
			spawner.y = -spawner.offY;
			spawner.angle = 256;
			break;
		case 1:
			spawner.x = GAME_W + spawner.offX;
			spawner.y = GAME_H / 2;
			spawner.angle = 512;
			spawner.ints[8] += 256;
			break;
		case 2:
			spawner.x = GAME_W / 2;
			spawner.y = GAME_H + spawner.offY;
			spawner.angle = 768;
			spawner.ints[8] += 512;
			break;
		case 3:
			spawner.x = -spawner.offX;
			spawner.y = GAME_H / 2;
			spawner.angle = 0;
			spawner.ints[8] += 768;
			break;
	}
	bossMax = spawner.health;
	void updater(s16 i){
		if(enemies[i].bools[0]){

			// shoot
			switch(currentBossNum){
				case 0: patternOne(i); break;
				case 1: patternTwo(i); break;
				case 2: patternThree(i); break;
				case 3: patternFour(i); break;
				case 4: patternFive(i); break;
				case 5:
					if(enemies[i].health >= 75) patternSix(i);
					else patternSeven(i);
					break;
				case 6:
					if(enemies[i].health >= 75) patternEight(i);
					else patternNine(i);
					break;
				case 7:
					if(enemies[i].health >= 88) patternSix(i);
					else patternSeven(i);
					break;
				case 8:
					if(enemies[i].health >= 88) patternEight(i);
					else patternNine(i);
					break;
				case 9: patternTen(i); break;
			}

			// movement
			flipEnemy(i);

		} else if(enemies[i].clock > 0 && enemies[i].clock % 15 == 0){
			enemies[i].speed = fix16Sub(enemies[i].speed, FIX16(0.5));
			updateEnemyVel(i);
			if(enemies[i].speed <= 0){
				enemies[i].angle = enemies[i].ints[8];
				enemies[i].speed = FIX16(0.5);
				updateEnemyVel(i);
				enemies[i].bools[0] = TRUE;
				enemies[i].clock = -1;
			}
		}
	}
	void suicide(s16 i){
		if(currentBossNum == 9){
			won = TRUE;
			killPlayer();
		} else {
			VDP_clearTileMapRect(BG_A, 1, 2, 4, 1);
			currentBossNum++;
			enemyCount--;
			for(s16 x = 0; x < BOSS_TILE_WIDTH; x++) VDP_clearTileMapRect(BG_A, 6 + x, 2, 1, 1);
			for(s16 j = 0; j < BULLET_COUNT; j++) if(bullets[j].active){
				if(j < EXPLOSION_LIMIT / 2) spawnExplosion(bullets[j].pos.x, bullets[j].pos.y, 0);
				killBullet(j);
			}
		}
	}
	spawnEnemy(spawner, updater, suicide);
	enemyCount++;
}