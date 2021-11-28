#include <genesis.h>
#include <resources.h>

#include "main.h"
#include "controls.h"
#include "player.h"
#include "stage.h"
#include "start.h"


// background

void loadStartBg(){
	for(s16 y = 0; y < START_BG_HEIGHT; y++)
		for(s16 x = 0; x < START_BG_WIDTH; x++) VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, 0, 0, HALF_TILE), x, y);
}


// logo

void loadStartLogo(){
	VDP_drawImageEx(BG_A, &start, TILE_ATTR_FULL(PAL1, 0, 0, 0, 9), 1, 2, 0, DMA_QUEUE);
	for(s16 x = 0; x < START_BG_WIDTH; x++)
		for(s16 y = 23; y < START_BG_HEIGHT; y++) VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, 0, 0, 8), x, y);
	for(s16 y = 21; y < 23; y++) VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, 0, 0, 8), 0, y);
}


// menu

void loadStartMenu(){
	VDP_drawText(">", START_MENU_X - 1, START_MENU_Y);
	VDP_drawText("START", START_MENU_X, START_MENU_Y);
	// VDP_drawText("ABOUT", START_MENU_X, START_MENU_Y + 2);
	VDP_drawText("LIVES;", START_MENU_X, START_MENU_Y + 2);
}

void updateStartLives(){
	if(currentStartMenu == 1 && (ctrl.left || ctrl.right) && !updatingStartCount){
		if(ctrl.right && player.lives < 6) player.lives++;
		else if(ctrl.left && player.lives > 0) player.lives--;
		updatingStartCount = TRUE;
	} else if(!ctrl.left && !ctrl.right && updatingStartCount) updatingStartCount = FALSE;
	if(startCurrentLives != player.lives){
		startCurrentLives = player.lives;
		intToStr(startCurrentLives, startCurrentLivesStr, 1);
		VDP_drawText(startCurrentLivesStr, START_MENU_X + 6, START_MENU_Y + 2);
	}
}

void updateStartMenu(){
	if(currentStartMenu != lastStartMenu && !aboutShowing){
		VDP_clearTileMapRect(BG_A, START_MENU_X - 1, START_MENU_Y + lastStartMenu * 2, 1, 1);
		VDP_drawText(">", START_MENU_X - 1, START_MENU_Y + currentStartMenu * 2);
		lastStartMenu = currentStartMenu;
	}
	if((ctrl.up || ctrl.down) && !selectingStartMenu && !aboutShowing){
		currentStartMenu += ctrl.up ? -1 : 1;
		if(currentStartMenu > 1) currentStartMenu = 0;
		else if(currentStartMenu < 0) currentStartMenu = 1;
		selectingStartMenu = TRUE;
		XGM_startPlayPCM(SFX_MENU_SELECT, 1, SOUND_PCM_CH2);
	} else if(!ctrl.up && !ctrl.down && !ctrl.a && !ctrl.b && !ctrl.c && !ctrl.start && selectingStartMenu) selectingStartMenu = FALSE;
	updateStartLives();
}

void selectStartMenu(){
	selectingStartMenu = TRUE;
	switch(currentStartMenu){
		case 0:
			resetStart();
			XGM_startPlayPCM(SFX_START_GAME, 1, SOUND_PCM_CH2);
			loadGame();
			break;
		// case 1:
			// loadStartAbout();
			// XGM_startPlayPCM(SFX_MENU_CHOOSE, 1, SOUND_PCM_CH2);
			// break;
	}
}


// about

void loadStartAbout(){
	aboutShowing = TRUE;
	for(s8 x = 0; x < START_BG_WIDTH; x++)
		for(s8 y = 0; y < START_BG_HEIGHT; y++)
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL2, 0, 0, 0, FULL_TILE), x, y);
	VDP_clearTileMapRect(BG_A, 0, 0, START_BG_WIDTH, START_BG_HEIGHT);
	aboutY = 2;
	VDP_drawText("the story so far", 8, aboutY);
	aboutY += 2;
	VDP_drawText("MOMOYO IS YOUR REGULAR HARD-", 2, aboutY); aboutY++;
	VDP_drawText("WORKING CENTIPEDE WHO LIVES IN", 1, aboutY); aboutY++;
	VDP_drawText("THE MOMENT... BUT DOES HER NEW", 1, aboutY); aboutY++;
	VDP_drawText("BOSS HAVE HER MAKING NEW PLANS", 1, aboutY); aboutY += 2;
	VDP_drawText("USE HER TOOLS OVER 20 STAGES &", 1, aboutY); aboutY++;
	VDP_drawText("DIG UP SOME SWEETS TO BRING TO", 1, aboutY); aboutY++;
	VDP_drawText("##MEGUMU##", 11, aboutY);
	aboutY += 2;
	VDP_drawText("how to play", 1, aboutY); aboutY += 2;
	VDP_drawText("d=pad  MOVE", 1, aboutY); aboutY++;
	VDP_drawText("a      SHOOT", 1, aboutY); aboutY++;
	VDP_drawText("b      FOCUS/SLOW", 1, aboutY); aboutY++;
	VDP_drawText("c      BOMB", 1, aboutY); aboutY++;
	VDP_drawText("start  PAUSE", 1, aboutY);
	aboutY += 3;
	VDP_drawText("EVERYTHING BY T.BODDY", 1, aboutY); aboutY++;
	VDP_drawText("FOR touhou pride game jam iii", 1, aboutY); aboutY++;
	VDP_drawText("TOUHOU PROJECT COPYRIGHT ZUN?", 1, aboutY); aboutY += 2;
	VDP_drawText("press any button to go back", 1, aboutY); aboutY++;
	startClock = SEGA_LIMIT + 120;
}

void startGoBack(){
	selectingStartMenu = TRUE;
	aboutShowing = FALSE;
	currentStartMenu = 0;
	// VDP_clearTileMapRect(BG_B, 0, 0, START_BG_WIDTH, START_BG_HEIGHT);
	VDP_clearTileMapRect(BG_A, 0, 0, START_BG_WIDTH, START_BG_HEIGHT);
	startCurrentLives = -1;
	// for(s16 y = 0; y < START_BG_HEIGHT; y++)
	// 	for(s16 x = 0; x < START_BG_WIDTH; x++) VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL2, 0, 0, 0, FULL_TILE), x, y);
	loadStartBg();
	loadStartLogo();
	loadStartMenu();
	loadStartScore();
	loadStartCredit();
}

void updateStartAbout(){
	if(startClock % 120 == 0) VDP_drawImageEx(BG_B, &startAbout, TILE_ATTR_FULL(PAL1, 0, 0, 0, 64), 22, 12, 0, DMA_QUEUE);
	else if(startClock % 120 == 60) VDP_drawImageEx(BG_B, &startAbout2, TILE_ATTR_FULL(PAL1, 0, 0, 0, 128), 22, 12, 0, DMA_QUEUE);
}


// score & credit

void loadStartScore(){
	intToStr(highScore, startHighScoreStr, 7);
	VDP_drawText("HI", 1, 26);
	VDP_drawText(startHighScoreStr, 4, 26);
}

void loadStartCredit(){
	VDP_drawText("11.2021 T.BODDY", 16, 26);
}



// main loop

void loadStart(){
	player.lives = 3;
	transitionX = 0;

	for(s16 x = 0; x < START_BG_WIDTH; x++){
		for(s16 y = 0; y < START_BG_HEIGHT; y++){
			VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL2, 0, 0, 0, FULL_TILE), x, y);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL2, 0, 0, 0, FULL_TILE), x, y);
		}
	}
	segaImage1 = SPR_addSprite(&startBoddy1, SEGA_X - 56, SEGA_Y, TILE_ATTR(PAL1, 0, 0, 0));
	segaImage2 = SPR_addSprite(&startBoddy2, SEGA_X, SEGA_Y, TILE_ATTR(PAL1, 0, 0, 0));
	VDP_loadTileSet(table.tileset, 8, DMA);

	// VDP_setScreenWidth256();
	// resetStart();
	// loadGame();
}

void resetStart(){
	VDP_clearTileMapRect(BG_A, 0, 0, START_BG_WIDTH, START_BG_HEIGHT);
	for(s16 x = 0; x < START_BG_WIDTH; x++)
		for(s16 y = 0; y < START_BG_HEIGHT; y++)
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL2, 0, 0, 0, FULL_TILE), x, y);
}

void updateStart(){
	if(startClock == 0) loadStart();
	if(startClock >= SEGA_LIMIT + 15){
		updateStartMenu();
		if((ctrl.a || ctrl.b || ctrl.c || ctrl.start) && aboutShowing && !selectingStartMenu) startGoBack();
		else if((ctrl.a || ctrl.start) && !aboutShowing && !selectingStartMenu) selectStartMenu();
		if(aboutShowing) updateStartAbout();
	} else if(startClock == SEGA_LIMIT - 25){
		for(s8 y = 0; y < START_BG_HEIGHT; y++)
			for(s8 x = -1; x < START_BG_WIDTH; x++) VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL2, 1, 0, 0, LEAST_TILE), x, y);
	} else if(startClock == SEGA_LIMIT - 20){
		for(s8 y = 0; y < START_BG_HEIGHT; y++)
			for(s8 x = -1; x < START_BG_WIDTH; x++) VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL2, 1, 0, 0, FULL_TILE), x, y);
	} else if(startClock == SEGA_LIMIT - 15){
		SPR_releaseSprite(segaImage1);
		SPR_releaseSprite(segaImage2);
	} else if(startClock == SEGA_LIMIT - 5){
		VDP_setScreenWidth256();
		loadStartBg();
	} else if(startClock == SEGA_LIMIT){
		for(s8 y = 0; y < START_BG_HEIGHT; y++)
			for(s8 x = -1; x < START_BG_WIDTH; x++) VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL2, 1, 0, 0, HALF_TILE), x, y);
	} else if(startClock == SEGA_LIMIT + 5){
		VDP_clearTileMapRect(BG_A, 0, 0, START_BG_WIDTH, START_BG_HEIGHT);
		// VDP_clearTileMapRect(BG_B, 0, 0, START_BG_WIDTH, START_BG_HEIGHT);
	} else if(startClock == SEGA_LIMIT + 10){
		loadStartLogo();
		loadStartMenu();
		loadStartScore();
		loadStartCredit();
		XGM_startPlay(&bgmStart);
	}
	startClock++;
	if(startClock >= 1800) startClock = SEGA_LIMIT + 120;
}