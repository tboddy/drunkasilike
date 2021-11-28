// chrome

// wave

char waveStr[4];
char debugStr1[8];
char debugStr2[8];
char debugStr3[8];
char highScoreStr[12];
char currentScoreStr[12];
char currentWaveStr[8];

char instructionStr[16];


// label

#define LABEL_COUNT 8
#define EXTEND 250000
#define MAX_LIVES 13

#define PAUSED_X 13
#define PAUSED_Y 14

struct label {
	bool active;
	s16 clock, x, y, w;
};
struct label labels[LABEL_COUNT];

void spawnLabel(fix16, fix16),
	updateLabels();

bool extended, chromePaused;


#define LIVES_X 9

#define FRAME_I 32
#define BOSS_I FRAME_I + 5
#define BOSS_TILE_WIDTH 25
#define BOSS_TILE_PX BOSS_TILE_WIDTH * 8

#define BOSS_LABEL_LIMIT 60 * 2.5
#define BOSS_LABEL_X 11
#define BOSS_LABEL_Y 14

s16 lastBoss, bossLimit, lastLives, bossLabelClock, gameOverY, nextExtend;
s32 lastScore;

void loadFrame(),
	loadScore(),
	loadHud(),
	loadInstructions(s16),
	loadBossLabel(),
	clearBossLabel(),
	loadGameOver();


void updateBoss(),
	updateScore(),
	updateHud(),
	updateGameOver(),
	updatePaused();

// loop

void loadChrome(),
	updateChrome();