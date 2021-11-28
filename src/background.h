// bg

#define BG_WIDTH 40
#define BG_HEIGHT 28
#define BG_SIZE -64

#define BG_I 64


// scrolling

Vect2D_f16 bgPos;
Sprite* groundImage;


// load bg

void loadBg(),
	loadFg();


// loop

void loadBackground(),
	updateBackground();