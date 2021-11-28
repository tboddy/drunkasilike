// stage
#define STAGE_COUNT_INT 8

s16 stageInts[STAGE_COUNT_INT];
Vect2D_f16 stageVectors[STAGE_COUNT_INT];

void (*currentWave)();

// wave

#define WAVE_LIMIT 15
#define WAVE_MAX 60 * WAVE_LIMIT

#define OVERLAY_I 128

#define FAST_BULLET_MOD FIX16(0.5)

s16 waveClock, bossHealth, bossMax, currentWaveNum, transitionClock,
	transitionX, transitionY, currentBossNum;
bool bossActive, fastBullets, suicideBullets, happyHour;
void updateWave(),
	checkWave(),
	transitionWave();


// loop

void loadStage(),
	updateStage();