#ifndef GAME_PROCESS_H_
#define GAME_PROCESS_H_

#include <sstream>

//UTIL HEADERS
#include "../util/util.h"
#include "../util/render_pipe.h"
#include "../util/timer.h"
#include "../util/constants.h"

//TEXTURES HEADERS
#include "../texture/LTexture.h"

//ENTITY HEADERS
#include "../entity/ui.h"
#include "../entity/ship.h"
#include "../entity/planet.h"
#include "../entity/enemy.h"
#include "../entity/orbit.h"
#include "../entity/health_module.h"
#include "../entity/obj_orbit.h"
#include "../entity/obj_health.h"
#include "../entity/ui_killbar.h"

enum GAME_STATES {
  MENU,
  PLAY,
  QUIT
};

GAME_STATES game_function(
  GAME_STATES state, 
  Render_pipe&, 
  UI&
);

GAME_STATES process_gameplay(
  Render_pipe&, 
  UI&
);

GAME_STATES process_menu(
  Render_pipe&, 
  UI&
);

void process_key(
  SDL_Event&, 
  Ship&, 
  Enemy* enemy_arr, 
  Orbit& orb
);

void process_menu_key(
  SDL_Event&,
  GAME_STATES&,
  int& active_button
);

void swap_buttons_colors(
  Render_pipe&, 
  int active_button, 
  int prev_button,
  LTexture& play, 
  LTexture& quit
);

GAME_STATES get_state(int active_button);

void add_life(
  Planet& pl, 
  Ship& sd
);

bool game_is_running(
  const Ship&, 
  const Planet&
);

#endif /* GAME_PROCESS_H_ */
