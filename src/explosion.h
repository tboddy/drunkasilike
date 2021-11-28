// explosion

#define EXPLOSION_LIMIT 12
#define EXPLOSION_OFFSET FIX16(12)

struct explosion {
	bool active;
	s16 clock;
	Sprite* image;
};

struct explosion explosions[EXPLOSION_LIMIT];

void spawnExplosion(fix16, fix16, s16),
	updateExplosion(s16),
	updateExplosions();