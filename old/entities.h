// entities

#define ENTITY_COUNT 192
#define COUNT_INT 8
#define COLLIDER_INT 4

struct entity {
	bool active, colliding;
	Vect2D_f16 pos, nextPos, lastPos, nextPosCheck, vel, col;
	s16 type, subType, angle, health, offX, offY, clock;
	fix16 speed;
	bool bools[COUNT_INT];
	s16 ints[COUNT_INT];
	fix16 fixes[COUNT_INT];
	Vect2D_f16 vectors[COUNT_INT];
	bool colliders[COLLIDER_INT];
	Vect2D_f16 colliderV[COLLIDER_INT];
	Sprite* image;
	void (*updater)(s16);
};

struct entity ent[ENTITY_COUNT];


// types

#define TYPE_PLAYER 0
#define TYPE_ENEMY 1
#define TYPE_BULLET 2
#define TYPE_BLOCK 3
#define TYPE_EXPLOSION 4
#define TYPE_POWERUP 5

s16 p;


// spawn

struct spawner {
	fix16 x, y, vX, vY, speed;
	s16 type, subType, angle, health, offX, offY, anim;
	Vect2D_f16 vel;
	bool bools[COUNT_INT];
	s16 ints[COUNT_INT];
	fix16 fixes[COUNT_INT];
	Vect2D_f16 vectors[COUNT_INT];
	Sprite* image;
};

void spawn(struct spawner, void(*updater));


// tools

#define VEL_C(i, d) fix16Mul(d ? cosFix16(ent[i].angle) : sinFix16(ent[i].angle), ent[i].speed);
#define UPDATE_VEL(i) ent[i].vel.x = VEL_C(i, 1); ent[i].vel.y = VEL_C(i, 0);
fix16 honeSpeed;
Vect2D_f16 hPos;
Vect2D_f16 hone(Vect2D_f16, Vect2D_f16, fix16, s16);


// loop

bool foundCol;

void collideEntity(s16),
	updateEntity(s16),
	killEntity(s16),
	drawEntity(s16);


// main loop

void loadEntities(),
	updateEntities();