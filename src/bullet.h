// bullets

#define BULLET_COUNT 64

#define BULLET_LIMIT_X FIX16(0)
#define BULLET_LIMIT_W FIX16(GAME_W)
#define BULLET_LIMIT_Y FIX16(0)
#define BULLET_LIMIT_Z FIX16(GAME_H)

#define B_COUNT_INT 8

#define KICKBACK FIX16(4)
#define KICKBACK_NORMALIZE fix16Mul(KICKBACK, FIX16(0.707))
#define KICKBACK_CHECK FIX16(8)

struct bulletSpawner {
	fix16 x, y, speed;
	Vect2D_f16 vel, hPos;
	s16 angle;
	Sprite* image;
	bool big, player;
	bool bools[B_COUNT_INT];
	s16 ints[B_COUNT_INT];
	fix16 fixes[B_COUNT_INT];
};

struct bullet {
	bool active, player;
	fix16 speed;
	fix32 dist;
	Vect2D_f16 pos, vel, off;
	s16 angle, clock;
	Sprite* image;
	void (*updater)(s16);
	bool bools[B_COUNT_INT];
	s16 ints[B_COUNT_INT];
	fix16 fixes[B_COUNT_INT];
};

struct bullet bullets[BULLET_COUNT];

bool bulletCollided;

fix32 bulletDist;

void spawnBullet(struct bulletSpawner, void(*updater)),
	killBullet(s16),
	collideBullet(s16),
	collideBulletWithEnemy(s16),
	collideBulletWithPlayer(s16),
	updateBullet(s16),
	updateBulletVel(s16),
	updateBullets();
