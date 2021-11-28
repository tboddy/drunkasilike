// enemies

#define ENEMY_COUNT 32
#define COUNT_INT 8

struct enemySpawner {
	fix16 speed;
	s16 angle, offX, offY, x, y, anim, health, frame;
	Vect2D_f16 vel;
	Sprite* image;
	bool electrode, drink, hulk, boss;
	bool bools[COUNT_INT];
	s16 ints[COUNT_INT];
	fix16 fixes[COUNT_INT];
};

struct enemy {
	bool active, electrode, drink, hulk, boss, flipped;
	fix16 speed;
	fix32 dist;
	Vect2D_f16 pos, vel, off, last;
	s16 angle, clock, health, flipClock;
	Sprite* image;
	void (*updater)(s16);
	void (*suicide)(s16);
	bool bools[COUNT_INT];
	s16 ints[COUNT_INT];
	fix16 fixes[COUNT_INT];
};
struct enemy enemies[ENEMY_COUNT];


// spawn

#define MAP_CHECK_X 16
#define MAP_CHECK_Y 14
bool mapCheck[MAP_CHECK_X][MAP_CHECK_Y];

#define ELECTRODE1_I 16
#define ELECTRODE2_I ELECTRODE1_I + 4
#define ELECTRODE3_I ELECTRODE2_I + 4

s16 eX, eY;
void setEnemyPos(s16 i);
void spawnEnemy(struct enemySpawner, void(*updater), void(*suicide)),
	spawnSuicideBullet(s16 i);
Vect2D_f16 velPos;

// update

fix32 enemyDist;
s16 enemyCount;

void updateEnemy(s16),
	flickerEnemy(s16),
	flipEnemy(s16),
	collideEnemy(s16),
	collideWall(s16),
	getDrink(s16),
	updateEnemyVel(s16),
	killEnemy(s16),
	updateEnemies();