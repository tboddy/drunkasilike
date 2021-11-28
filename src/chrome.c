// chrome

#include <genesis.h>
#include <resources.h>

#include "main.h"
#include "chrome.h"
#include "player.h"
#include "enemies.h"
#include "bullet.h"
#include "stage.h"


// load

void loadScore(){
	intToStr(currentScore, currentScoreStr, 7);
	VDP_drawText(currentScoreStr, 1, 1);
}

void loadHud(){
	intToStr(currentWaveNum == 0 ? 1 : currentWaveNum, currentWaveStr, 3);
	VDP_drawText("wave", 23, 1);
	VDP_drawText(currentWaveStr, 28, 1);
	for(s16 i = 0; i < player.lives; i++) VDP_drawText("#", LIVES_X + i, 1);
}

void loadInstructions(s16 offset){
	VDP_drawText(instructionStr, 1 + offset, 2);
}

void loadBossLabel(){
	bossLabelClock = BOSS_LABEL_LIMIT;
	VDP_drawText("BOSS  WAVE", BOSS_LABEL_X, BOSS_LABEL_Y);
}

void clearBossLabel(){
	VDP_clearTileMapRect(BG_A, BOSS_LABEL_X, BOSS_LABEL_Y, 10, 1);
}


// game over

void loadGameOver(){
	XGM_stopPlay();
	for(s16 i = 0; i < ENEMY_COUNT; i++) if(enemies[i].active) killEnemy(i);
	for(s16 i = 0; i < BULLET_COUNT; i++) if(bullets[i].active) killBullet(i);
	VDP_setHorizontalScroll(BG_B, 0);
	VDP_setVerticalScroll(BG_B, 0);
	for(s16 x = 0; x < 32; x++) for(s16 y = 0; y < 28; y++)
		if(x % 4 == 0 && y % 4 == 0) VDP_drawImageEx(BG_B, won ? &overlayGood : &overlayBad, TILE_ATTR_FULL(PAL2, 1, 0, 0, OVERLAY_I), x, y, 0, DMA);
	gameOverY = currentScore > highScore ? 9 : 10;
	won ? VDP_drawText("YOU  WIN", 12, gameOverY) : VDP_drawText("GAME  OVER", 11, gameOverY);
	gameOverY += 2;
	VDP_drawText("YOUR SCORE", 11, gameOverY);
	gameOverY += 1;
	VDP_drawText(currentScoreStr, 12, gameOverY);
	if(currentScore > highScore){
		gameOverY += 2;
		VDP_drawText("NEW HIGH SCORE", 9, gameOverY);
		highScore = currentScore;
	}
	gameOverY += 3;
	VDP_drawText("THANK YOU FOR PLAYING#", 5, gameOverY);
	gameOverY += 1;
	VDP_drawText("PRESS ANY BUTTON", 8, gameOverY);
	XGM_startPlayPCM(won ? SFX_BEAT_GAME : SFX_GAME_OVER, 0, SOUND_PCM_CH2);
}


// labels

void spawnLabel(fix16 x, fix16 y){
	s16 i = -1;
	for(s16 j = 0; j < LABEL_COUNT; j++) if(!labels[j].active && i == -1) i = j;
	labels[i].active = TRUE;
	labels[i].clock = 0;
	labels[i].x = fix16ToRoundedInt(fix16Div(x, FIX16(8))) - 3;
	labels[i].y = fix16ToRoundedInt(fix16Div(y, FIX16(8)));
	labels[i].w = 5;
	VDP_drawText("+1000", labels[i].x, labels[i].y);
}

void updateLabels(){
	for(s16 i = 0; i < LABEL_COUNT; i++) if(labels[i].active){
		labels[i].clock++;
		if(labels[i].clock >= 90){
			labels[i].active = FALSE;
			VDP_clearText(labels[i].x, labels[i].y, labels[i].w);
		}
	}
}


// update

void updateBoss(){
	if(bossHealth > 0){
		if(lastBoss != bossHealth){
			VDP_drawText("boss", 1, 2);
			bossLimit = fix16ToInt(fix16Mul(fix16Div(FIX16(bossHealth), FIX16(bossMax)), FIX16(BOSS_TILE_PX))) / 8; // percent of 100
			for(s16 x = 0; x < BOSS_TILE_WIDTH; x++){
				if(x < bossLimit) VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, 0, 0, BOSS_I), 6 + x, 2);
				else VDP_clearTileMapRect(BG_A, 6 + x, 2, 1, 1);
			}
			lastBoss = bossHealth;
		}
	} else {
		lastBoss = 0;
		bossActive = FALSE;
	}
	if(bossLabelClock > 0){
		bossLabelClock--;
		if(bossLabelClock <= 0) clearBossLabel();
	}
}

void updateScore(){
	if(currentScore > lastScore && enemyCount > 0){
		lastScore = currentScore;
		if(currentScore >= nextExtend){
			if(player.lives < MAX_LIVES){
				player.lives++;
				nextExtend += extend;
			} else currentScore += 10000;
		}
		intToStr(currentScore, currentScoreStr, 7);
		VDP_clearText(1, 1, 7);
		VDP_drawText(currentScoreStr, 1, 1);
	}
}

void updateHud(){
	if(player.lives != lastLives){
		lastLives = player.lives;
		VDP_clearTileMapRect(BG_A, LIVES_X, 1, 12, 1);
		for(s16 i = 0; i < player.lives; i++) VDP_drawText("#", LIVES_X + i, 1);
	}
}

void updateGameOver(){
	if(transitionX == 32 || transitionX == -4) transitionY += 4;
	if(gameOverClock < 62){
		VDP_drawImageEx(BG_A, won ? &overlayGood : &overlayBad, TILE_ATTR_FULL(PAL2, 1, 0, 0, OVERLAY_I), transitionX, transitionY, 0, DMA);
		transitionX += transitionY % 8 == 0 ? 4 : -4;
		gameOverClock++;
		if(gameOverClock == 62) loadGameOver();
	} 
}

void updatePaused(){
	if(paused && !chromePaused){
		VDP_drawText("paused", PAUSED_X, PAUSED_Y);
		chromePaused = TRUE;
		XGM_pausePlay();
	} else if(!paused && chromePaused){
		VDP_clearText(PAUSED_X, PAUSED_Y, 6);
		chromePaused = FALSE;
		XGM_resumePlay();
	}
}


// loop

void loadChrome(){
	VDP_loadTileSet(bossBar.tileset, BOSS_I, DMA);
	loadScore();
	loadHud();
}

void updateChrome(){
	if(gameOver) updateGameOver();
	else {
		updateBoss();
		updateScore();
		updateLabels();
		updateHud();
		updatePaused();
	}

	// intToStr(enemyCount, debugStr1, 4);
	// VDP_drawText(debugStr1, 0, 27);
}