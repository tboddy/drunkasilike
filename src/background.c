// background

#include <genesis.h>
#include <resources.h>

#include "main.h"
#include "stage.h"
#include "background.h"


// scrolling
void updateBackground(){
	if(clock % 4 == 0 && !gameOver && !paused){
		bgPos.x -= 1;
		if(clock % 8 == 0) bgPos.y -= 1;
		if(bgPos.x < BG_SIZE) bgPos.x = 0;
		if(bgPos.y < BG_SIZE) bgPos.y = 0;
		VDP_setHorizontalScroll(BG_B, bgPos.x);
		VDP_setVerticalScroll(BG_B, bgPos.y);
	}
}


// load room

void loadBg(){
	if(currentWaveNum > 5 && currentWaveNum <= 10) groundImage = &ground2;
	else if(currentWaveNum > 10 && currentWaveNum <= 15) groundImage = &ground3;
	else if(currentWaveNum > 15 && currentWaveNum <= 20) groundImage = &ground4;
	else if(currentWaveNum > 20 && currentWaveNum <= 25) groundImage = &ground5;
	else if(currentWaveNum > 25 && currentWaveNum <= 30) groundImage = &ground6;
	else if(currentWaveNum > 30 && currentWaveNum <= 35) groundImage = &ground7;
	else if(currentWaveNum > 35 && currentWaveNum <= 40) groundImage = &ground8;
	else if(currentWaveNum > 40 && currentWaveNum <= 45) groundImage = &ground9;
	else if(currentWaveNum > 45) groundImage = &ground10;
	else groundImage = &ground1;
	for(s16 x = 0; x < BG_WIDTH; x++) for(s16 y = 0; y < BG_HEIGHT; y++)
		if(x % 8 == 0 && y % 8 == 0) VDP_drawImageEx(BG_B, groundImage, TILE_ATTR_FULL(PAL2, 0, 0, 0, BG_I), x, y, 0, DMA);
}

void loadFg(){
	for(s16 x = 0; x < BG_WIDTH; x++) for(s16 y = 0; y < BG_HEIGHT; y++){
		if ((y == 0 && x == 0) || (x == 31 && y == 0) || (y == 27 && x == 31) || (y == 27 && x == 0))
			VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL2, 0, 0, 0, HALF_TILE), x, y);
		else if(y == 0 || y == 27 || x == 0 || x == 31)
			VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL2, 0, 0, 0, MOST_TILE), x, y);
	}
}

void loadBackground(){
	VDP_setScrollingMode(HSCROLL_PLANE, HSCROLL_PLANE);
	loadBg();
	loadFg();
}