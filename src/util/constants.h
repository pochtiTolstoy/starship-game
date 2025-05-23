#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include "util.h"

static const int SCREEN_WIDTH = 1920;
static const int SCREEN_HEIGHT = 1080;

static const int NUM_SHIP_TEXTURES_1 = 6;
static const int NUM_SHIP_TEXTURES_2 = 6;

static const int NUM_ENEMY_TEXTURES = 1;
static const int NUM_ENEMY_ON_MAP = 24;
static const int NUM_UI_TEXTURES = 6;
static const int NUM_ORBIT_TEXTURES = 2;
static const int NUM_HEALTH_MODULE_TEXTURES = 2;
static const int NUM_OBJ_HEALTH_ON_MAP = 2;
static const int NUM_BACKGROUNDS = 3;
static const int NUM_LEVEL_IMAGES = 2;
static const int NUM_FONTS = 3;
static const int COORDS_SYNC = 90;
static const int SPAWN_ENEMY_X = -400;
static const int DEGREES_IN_CIRCLE = 360;
static const int DEGREES_IN_HALF_CIRCLE = 180;
static const int SHIP_HITBOX = 128;
static const int PLANET_HITBOX = 240;
static const int HEALTH_HITBOX = 64;
static const int SHIFT_HEART_PLANET_Y = 185;
static const int SHIFT_HEART_SHIP_Y = 30;
static const double MOVE_ANGULAR = 15;
static const int MOVE_LEN = 30;
static const int SCREEN_FPS = 60;
static const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

enum GAME_STATES {
  MENU,
  PLAY_MENU,
  LVL1,
  LVL2,
  LVL1_LOSE,
  LVL2_LOSE,
  HELP,
  WIN,
  LOSE,
  QUIT,
  RETRY
};

/* === Safe to change === */

static const int RAND_SPAWN = 1200;
static const int RAND_SPAWN_FIRST = 1200;
static const int KILLS_TO_WIN = 200;

// EASY
/*
static const int RAND_SPAWN_FIRST = 3000;
static const int RAND_SPAWN = 3000;
static const int KILLS_TO_WIN = 50;
*/

static const SDL_Color color_main_pass = {0, 192, 248, 0xFF};
static const SDL_Color color_shadow_pass = {7, 63, 147, 0xFF};
static const SDL_Color color_main_act = {163, 234, 255, 0xFF};
static const SDL_Color color_shadow_act = {0, 75, 187, 0xFF};

// Enemy movements
static const int ENEMY_SPEED_LEVELS = 100;
static const int ENEMY_FRAMERATE_LEVELS = 1;

/* === File paths === */

static const char *FILE_PATHS_SHIP_1[NUM_SHIP_TEXTURES_1] = {
    "res/pics/ship1Big.png",       "res/pics/ship1moveBig.png",
    "res/pics/ship_shoot1Big.png", "res/pics/ship_back1Big.png",
    "res/pics/ship_reloadBig.png", "res/pics/triple_shoot_test.png"};

static const char *FILE_PATHS_SHIP_2[NUM_SHIP_TEXTURES_2] = {
    "res/pics/eva_ship_main.png", "res/pics/eva_ship_main.png",
    "res/pics/eva_ship_main.png", "res/pics/eva_ship_main.png",
    "res/pics/eva_ship_main.png", "res/pics/eva_ship_main.png"};

static const char *FILE_PATHS_ENEMY[NUM_ENEMY_TEXTURES] = {
    "res/pics/meteor1Big.png"
    /*"res/pics/eva_ship.png"*/
};

static const char *FILE_PATHS_UI[NUM_UI_TEXTURES] = {
    "res/pics/heartBig.png",     "res/pics/heartBlackBig.png",
    "res/pics/ui_shootBig.png",  "res/pics/ui_shootBlackBig.png",
    "res/pics/orbitElement.png", "res/pics/ui_shootBlue.png"};

static const char *FILE_PATHS_ORBIT[NUM_ORBIT_TEXTURES] = {
    "res/pics/orbitDefault.png", "res/pics/orbitMove.png"};

static const char *FILE_PATH_HEALTH_MODULE[NUM_HEALTH_MODULE_TEXTURES] = {
    "res/pics/healthModule.png", "res/pics/healthModuleBack.png"};

/*
static const char* FILE_PATH_BACKGROUND[NUM_BACKGROUNDS] = {
  "res/pics/planet1DarkBig.png",
  "res/pics/menuBack.png"
};
*/

static const char *FILE_PATH_BACKGROUND[NUM_BACKGROUNDS] = {
    "res/pics/planet1Big.png", "res/pics/menuBack.png",
    "res/pics/eva_space.png"};

static const char *FILE_PATH_MINE = "res/pics/mine.png";

// static const char* FILE_PATH_FONT = "res/Mx437_Acer710_Mono.ttf";
static const char *FILE_PATH_FONT = "res/starship_font.ttf";

static const char *FILE_PATHS_LEVEL_IMAGES[NUM_LEVEL_IMAGES] = {
    "res/pics/lvl1.png", "res/pics/lvl2.png"};

/* === Text === */
static const char *HELP_TEXT = "USE W, A, S, D TO MOVE YOUR SHIP,\n\n"
                               "USE SPACE TO SHOOT,\n\n"
                               "USE E TO TURN 180 DEGREES.\n\n"
                               "SOMETIMES YOU CAN TAKE AN ORBITAL PROBE,\n\n"
                               "PRESS M TO ACCELERATE ORBITAL PROBE.\n\n"
                               "PRESS ESC TO OPEN MENU.";

#endif /* CONSTANTS_H_ */
