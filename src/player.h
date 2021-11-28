// player

#define PLAYER_INIT_X FIX16(GAME_W / 2)
#define PLAYER_INIT_Y FIX16(GAME_H / 2)

#define PLAYER_Y_LIMIT FIX16(GAME_H - 48)

#define PLAYER_SPEED FIX16(1.9)

#define PLAYER_LIMIT_X FIX16(7)
#define PLAYER_LIMIT_W fix16Sub(FIX16(GAME_W), FIX16(6))
#define PLAYER_LIMIT_Y FIX16(8)
#define PLAYER_LIMIT_Z fix16Sub(FIX16(GAME_H), FIX16(8))

#define INVINCIBLE_LIMIT 180
#define INVINCIBLE_MID INVINCIBLE_LIMIT / 2

// directions

#define PLAYER_VEL_BL_X fix16Mul(cosFix16(384), PLAYER_SPEED)
#define PLAYER_VEL_BL_Y fix16Mul(sinFix16(384), PLAYER_SPEED)

#define PLAYER_VEL_BR_X fix16Mul(cosFix16(128), PLAYER_SPEED)
#define PLAYER_VEL_BR_Y fix16Mul(sinFix16(128), PLAYER_SPEED)

#define PLAYER_VEL_TL_X fix16Mul(cosFix16(640), PLAYER_SPEED)
#define PLAYER_VEL_TL_Y fix16Mul(sinFix16(640), PLAYER_SPEED)

#define PLAYER_VEL_TR_X fix16Mul(cosFix16(896), PLAYER_SPEED)
#define PLAYER_VEL_TR_Y fix16Mul(sinFix16(896), PLAYER_SPEED)

#define PLAYER_START_TIME 45


// struct

struct player {
	Vect2D_f16 pos, vel, off;
	s16 angle, moveClock, shotClock, clock, invincibleClock, shotAngle,
		lives, bombs;
	fix16 moveSpeed, shotSpeed;
	fix32 dist, wallDist, wallDist2;
	bool invincible;
	Sprite* image;
};
struct player player;


// spawn

void spawnPlayer();


// shoot

#define PLAYER_BULLET_COUNT 16
s16 playerBulletIndexes[PLAYER_BULLET_COUNT];

void spawnPlayerBullet(),
	updatePlayerBullet(s16);


// update

#define SHOT_INTERVAL 10

void movePlayer(),
	collidePlayer(),
	updateInvincible(),
	updatePlayerShot(),
	killPlayer();


// loop

void updatePlayer(),
	loadPlayer();