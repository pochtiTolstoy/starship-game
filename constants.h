#ifndef CONSTANTS_H_
#define CONSTANTS_H_

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
const int NUM_SHIP_TEXTURES = 5;
const int NUM_ENEMY_TEXTURES = 1;
const int NUM_ENEMY_ON_MAP = 24;
const int NUM_UI_TEXTURES = 5;
const int COORDS_SYNC = 90;
const int SPAWN_ENEMY_X = -400;
const int DEGREES_IN_CIRCLE = 360;
const int DEGREES_IN_HALF_CIRCLE = 180;
const int SHIP_HITBOX = 128;
const int HEALTH_HITBOX = 64;
const int SHIFT_HEART_PLANET_Y = 185;
const int SHIFT_HEART_SHIP_Y = 30;
const double MOVE_ANGULAR = 15;

/* === Safe to change === */

const int RAND_SPAWN = 1000;
const int RAND_SPAWN_FIRST = 1000;
const int KILLS_TO_WIN = 60;

//Enemy movements
const int ENEMY_SPEED_LEVELS = 3;
const int ENEMY_FRAMERATE_LEVELS = 6;




/* === File paths === */

const char* file_paths_ship[NUM_SHIP_TEXTURES] = {
	"pics/ship1Big.png",
	"pics/ship1moveBig.png",
	"pics/ship_shoot1Big.png",
	"pics/ship_back1Big.png",
	"pics/ship_reloadBig.png"
};

const char* file_paths_enemy[NUM_ENEMY_TEXTURES] = {
	"pics/meteor1Big.png"
};

const char* file_paths_ui[NUM_UI_TEXTURES] = {
	"pics/heartBig.png",
	"pics/heartBlackBig.png",
	"pics/ui_shootBig.png",
	"pics/ui_shootBlackBig.png",
	"pics/obj_healthBig.png"
};

const char* file_path_font = "res/Mx437_Acer710_Mono.ttf";

#endif /* CONSTANTS_H_ */
