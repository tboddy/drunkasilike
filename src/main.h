// main

void EMPTY(s16);

#define WIN_W 256
#define WIN_H 224

// #define GAME_W 176
// #define GAME_H 224
#define GAME_W 256
#define GAME_H 224

#define GAME_X 0
#define GAME_Y 0

#define CLOCK_LIMIT 7200

#define SFX_MENU_SELECT 64
#define SFX_MENU_CHOOSE 65
#define SFX_START_GAME 66
#define SFX_PLAYER_SHOT 67
#define SFX_ZONE_OVER 68

#define SFX_BULLET_1 69

#define SFX_EXPLOSION_1 73
#define SFX_EXPLOSION_2 74
#define SFX_EXPLOSION_3 75

#define SFX_GAME_OVER 76
#define SFX_BEAT_GAME 77

s16 clock;
u32 currentScore, highScore, extend;

bool started, paused, gameOver, won, gameOverClock, pausing;

// game loop

void loadGame(),
	updateGame(),
	resetGame();


// helpers

#define S_ATTR TILE_ATTR(PAL1, 0, 0, 0)

fix16 honeSpeed;
Vect2D_f16 hPos;
Vect2D_f16 hone(Vect2D_f16, Vect2D_f16, fix16, s16);

// resources

#define LEAST_TILE 2
#define HALF_TILE LEAST_TILE + 1
#define MOST_TILE HALF_TILE + 1
#define FULL_TILE MOST_TILE + 1

void loadResources();


// main loop

int main();