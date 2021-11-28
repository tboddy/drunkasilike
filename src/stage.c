// stage

#include <genesis.h>
#include <resources.h>

#include "main.h"
#include "enemies.h"
#include "bullet.h"
#include "player.h"
#include "background.h"
#include "enemytypes.h"
#include "chrome.h"
#include "stage.h"

// waves

void waveOne(){
	for(s16 i = 0; i < 4; i++) spawnGrunt();
	for(s16 i = 0; i < 2; i++) spawnElectrode();
	spawnSushi();
	strcpy(instructionStr, "shoot drinks + grab sushi!");
	loadInstructions(2);
}

void waveTwo(){
	for(s16 i = 0; i < 4; i++){
		spawnGrunt();
		spawnElectrode();
	}
	spawnHulk();
	for(s16 i = 0; i < 2; i++) spawnSushi();
	strcpy(instructionStr, "pickles are invincible");
	loadInstructions(4);
}

void waveThree(){
	for(s16 i = 0; i < 6; i++){
		spawnGrunt();
		spawnElectrode();
	}
	for(s16 i = 0; i < 2; i++) spawnHulk();
	spawnEnforcer();
	for(s16 i = 0; i < 3; i++) spawnSushi();
	strcpy(instructionStr, "take out wine glasses first!");
	loadInstructions(1);
}

void waveFour(){
	for(s16 i = 0; i < 8; i++) spawnGrunt();
	for(s16 i = 0; i < 6; i++) spawnElectrode();
	for(s16 i = 0; i < 3; i++) spawnHulk();
	for(s16 i = 0; i < 2; i++) spawnEnforcer();
	for(s16 i = 0; i < 3; i++) spawnSushi();
	strcpy(instructionStr, "lanterns get in your way");
	loadInstructions(3);
}

void waveSix(){
	for(s16 i = 0; i < 8; i++) spawnGrunt();
	for(s16 i = 0; i < 6; i++) spawnElectrode();
	for(s16 i = 0; i < 3; i++) spawnHulk();
	for(s16 i = 0; i < 2; i++) spawnEnforcer();
	for(s16 i = 0; i < 3; i++) spawnSushi();
}

void waveSeven(){
	for(s16 i = 0; i < 3; i++) spawnTank();
	for(s16 i = 0; i < 4; i++) spawnEnforcer();
	for(s16 i = 0; i < 4; i++) spawnSushi();
	strcpy(instructionStr, "happy hour! sushi worth double");
	loadInstructions(0);
}

void waveEight(){
	for(s16 i = 0; i < 8; i++) spawnGrunt();
	for(s16 i = 0; i < 6; i++) spawnElectrode();
	for(s16 i = 0; i < 3; i++) spawnHulk();
	for(s16 i = 0; i < 3; i++) spawnEnforcer();
	for(s16 i = 0; i < 4; i++) spawnSushi();
}

void waveNine(){
	for(s16 i = 0; i < 16; i++) spawnGrunt();
	for(s16 i = 0; i < 1; i++) spawnHulk();
	for(s16 i = 0; i < 2; i++) spawnEnforcer();
	for(s16 i = 0; i < 3; i++) spawnSushi();
}

void bossWave(){
	spawnSake();
	loadBossLabel();
}

void waveTwelve(){
	for(s16 i = 0; i < 4; i++) spawnTank();
	for(s16 i = 0; i < 4; i++) spawnEnforcer();
	for(s16 i = 0; i < 4; i++) spawnSushi();
	strcpy(instructionStr, "happy hour! sushi worth double");
	loadInstructions(0);
}

void waveFourteen(){
	for(s16 i = 0; i < 4; i++) spawnGrunt();
	for(s16 i = 0; i < 3; i++) spawnElectrode();
	for(s16 i = 0; i < 6; i++) spawnHulk();
	for(s16 i = 0; i < 3; i++) spawnEnforcer();
	for(s16 i = 0; i < 4; i++) spawnSushi();
}

void waveNinteen(){
	for(s16 i = 0; i < 20; i++) spawnGrunt();
	for(s16 i = 0; i < 1; i++) spawnHulk();
	for(s16 i = 0; i < 2; i++) spawnEnforcer();
	for(s16 i = 0; i < 3; i++) spawnSushi();
}

void waveTwentyFour(){
	for(s16 i = 0; i < 8; i++) spawnHulk();
	for(s16 i = 0; i < 3; i++) spawnEnforcer();
	for(s16 i = 0; i < 3; i++) spawnSushi();
}

void waveThirtyOne(){
	waveEight();
	strcpy(instructionStr, "bullets are now 25% faster");
	loadInstructions(2);
}

void waveThirtyFour(){
	for(s16 i = 0; i < 8; i++) spawnGrunt();
	for(s16 i = 0; i < 8; i++) spawnHulk();
	spawnEnforcer();
	spawnTank();
	for(s16 i = 0; i < 3; i++) spawnSushi();
}

void waveThirtyNine(){
	for(s16 i = 0; i < 18; i++) spawnGrunt();
	for(s16 i = 0; i < 2; i++) spawnHulk();
	for(s16 i = 0; i < 2; i++) spawnEnforcer();
	for(s16 i = 0; i < 4; i++) spawnSushi();
	spawnTank();
}

void waveFourtyOne(){
	waveEight();
	strcpy(instructionStr, "watch for suicide bullets!");
	loadInstructions(2);
}

// wave switching

void checkWave(){
	if(enemyCount <= 0){
		for(s16 i = 0; i < ENEMY_COUNT; i++) if(enemies[i].active) killEnemy(i);
		for(s16 i = 0; i < BULLET_COUNT; i++) if(bullets[i].active) killBullet(i);
		currentWaveNum++;
		waveClock = -1;
		// XGM_stopPlay();
		XGM_startPlayPCM(SFX_ZONE_OVER, 0, SOUND_PCM_CH3);
		VDP_clearPlane(BG_A, TRUE);
		SPR_clear();
		transitionClock = 0;
		SPR_setVisibility(player.image, HIDDEN);
		switch(currentWaveNum){
			case 2: currentWave = &waveTwo; break;
			case 3: currentWave = &waveThree; break;
			case 4: currentWave = &waveFour; break;
			case 5: currentWave = &bossWave; break; // boss
			case 6: currentWave = &waveSix; break;
			case 7:
				happyHour = TRUE;
				currentWave = &waveSeven;
				break;
			case 8:
				happyHour = FALSE;
				currentWave = &waveEight;
				break;
			case 9: currentWave = &waveNine; break;
			case 10: currentWave = &bossWave; break; // boss
			case 11: currentWave = &waveEight; break;
			case 12:
				happyHour = TRUE;
				currentWave = &waveTwelve; break;
			case 13:
				happyHour = FALSE;
				currentWave = &waveEight; break;
			case 14: currentWave = &waveFourteen; break;
			case 15: currentWave = &bossWave; break; // boss
			case 16: currentWave = &waveEight; break;
			case 17:
				happyHour = TRUE;
				currentWave = &waveTwelve; break;
			case 18:
				happyHour = FALSE;
				currentWave = &waveEight; break;
			case 19: currentWave = &waveNinteen; break;
			case 20: currentWave = &bossWave; break; // boss
			case 21: currentWave = &waveEight; break;
			case 22:
				happyHour = TRUE;
				currentWave = &waveTwelve; break;
			case 23:
				happyHour = FALSE;
				currentWave = &waveEight; break;
			case 24: currentWave = &waveTwentyFour; break;
			case 25: currentWave = &bossWave; break; // boss
			case 26: currentWave = &waveEight; break;
			case 27:
				happyHour = TRUE;
				currentWave = &waveTwelve; break;
			case 28:
				happyHour = FALSE;
				currentWave = &waveEight; break;
			case 29: currentWave = &waveNinteen; break;
			case 30: currentWave = &bossWave; break; // boss
			case 31:
				fastBullets = TRUE;
				currentWave = &waveThirtyOne;
				break;
			case 32:
				happyHour = TRUE;
				currentWave = &waveTwelve; break;
			case 33:
				happyHour = FALSE;
				currentWave = &waveEight; break;
			case 34: currentWave = &waveThirtyFour; break;
			case 35: currentWave = &bossWave; break;
			case 36: currentWave = &waveEight; break;
			case 37:
				happyHour = TRUE;
				currentWave = &waveTwelve; break;
			case 38:
				happyHour = FALSE;
				currentWave = &waveEight; break;
			case 39: currentWave = &waveThirtyNine; break;
			case 40: currentWave = &bossWave; break;
			case 41:
				suicideBullets = TRUE;
				currentWave = &waveFourtyOne; break;
			case 42:
				happyHour = TRUE;
				currentWave = &waveTwelve; break;
			case 43:
				happyHour = FALSE;
				currentWave = &waveEight; break;
			case 44: currentWave = &waveThirtyFour; break;
			case 45: currentWave = &bossWave; break;
			case 46: currentWave = &waveEight; break;
			case 47:
				happyHour = TRUE;
				currentWave = &waveTwelve; break;
			case 48:
				happyHour = FALSE;
				currentWave = &waveEight; break;
			case 49: currentWave = &waveThirtyNine; break;
			case 50: currentWave = &bossWave; break;
		}
	}
}

void transitionWave(){
	waveClock = -1;
	if(transitionX == 32 || transitionX == -4) transitionY += 4;
	if(transitionClock < 62){
		VDP_drawImageEx(BG_A, &overlay, TILE_ATTR_FULL(PAL2, 1, 0, 0, OVERLAY_I), transitionX, transitionY, 0, DMA);
		transitionX += transitionY % 8 == 0 ? 4 : -4;
		transitionClock++;
	} else if(transitionClock < 124){
		if(transitionClock == 62){
			loadBg();
			transitionX = 0;
			transitionY = 0;
		}
		VDP_clearTileMapRect(BG_A, transitionX, transitionY, 4, 4);
		transitionX += transitionY % 8 == 0 ? 4 : -4;
		transitionClock++;
	} else {
		waveClock = 0;
		transitionClock = 0;
		transitionX = 0;
		transitionY = 0;
		player.invincible = FALSE;
		player.invincibleClock = 0;
		// SPR_setVisibility(player.image, VISIBLE);
		player.pos.x = PLAYER_INIT_X;
		player.pos.y = PLAYER_INIT_Y;
		loadFg();
		loadChrome();
		// XGM_startPlay(&bgmStage);
	}
}


// loop

void loadStage(){
	waveClock = 0;
	currentBossNum = 0;
	currentWaveNum = 1;
	currentWave = &waveOne;
	XGM_startPlay(&bgmStage);

	// currentBossNum = 7;
	// currentWaveNum = 35;
	// currentWave = &bossWave;
}

void updateStage(){
	if(!gameOver && !paused){
		if(waveClock == 0){
			SPR_setVisibility(player.image, HIDDEN);
			enemyCount = 0;
			for(s16 x = 0; x < MAP_CHECK_X; x++) for(s16 y = 0; y < MAP_CHECK_Y; y++)
				if(mapCheck[x][y]) mapCheck[x][y] = FALSE;
			currentWave();
		} else if(waveClock > 0){
			if(waveClock == PLAYER_START_TIME) SPR_setVisibility(player.image, VISIBLE);
			checkWave();
		}
		if(waveClock < 0) transitionWave();
		else waveClock++;
		if(waveClock >= CLOCK_LIMIT) waveClock = 60;
	}
}