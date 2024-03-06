#ifndef CONSTANTS_H_
#define CONSTANTS_H_

static const int SCREEN_WIDTH = 1920;
static const int SCREEN_HEIGHT = 1080;
static const int NUM_SHIP_TEXTURES = 5;
static const int NUM_ENEMY_TEXTURES = 1;
static const int NUM_ENEMY_ON_MAP = 24;
static const int NUM_UI_TEXTURES = 5;
static const int COORDS_SYNC = 90;
static const int SPAWN_ENEMY_X = -400;
static const int DEGREES_IN_CIRCLE = 360;
static const int DEGREES_IN_HALF_CIRCLE = 180;
static const int SHIP_HITBOX = 128;
static const int HEALTH_HITBOX = 64;
static const int SHIFT_HEART_PLANET_Y = 185;
static const int SHIFT_HEART_SHIP_Y = 30;
static const double MOVE_ANGULAR = 15;

/* === Safe to change === */

static const int RAND_SPAWN = 1000;
static const int RAND_SPAWN_FIRST = 1000;
static const int KILLS_TO_WIN = 60;

//Enemy movements
static const int ENEMY_SPEED_LEVELS = 3;
static const int ENEMY_FRAMERATE_LEVELS = 6;

/* === File paths === */

static const char* FILE_PATHS_SHIP[NUM_SHIP_TEXTURES] = {
	"res/pics/ship1Big.png",
	"res/pics/ship1moveBig.png",
	"res/pics/ship_shoot1Big.png",
	"res/pics/ship_back1Big.png",
	"res/pics/ship_reloadBig.png"
};

static const char* FILE_PATHS_ENEMY[NUM_ENEMY_TEXTURES] = {
	"res/pics/meteor1Big.png"
};

static const char* FILE_PATHS_UI[NUM_UI_TEXTURES] = {
	"res/pics/heartBig.png",
	"res/pics/heartBlackBig.png",
	"res/pics/ui_shootBig.png",
	"res/pics/ui_shootBlackBig.png",
	"res/pics/obj_healthBig.png"
};

static const char* FILE_PATH_BACKGROUND = "res/pics/planet1Big.png";
static const char* FILE_PATH_FONT = "res/Mx437_Acer710_Mono.ttf";

#endif /* CONSTANTS_H_ */