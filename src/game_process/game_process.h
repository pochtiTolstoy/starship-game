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
#include "../entity/text_box.h"
#include "../entity/level_images.h"

//GAME PAGES HEADERS
#include "game_levels.h"

void process_level_menu_key(
  SDL_Event& e,
  GAME_STATES& state,
  int& active_button
);

void change_active_button_level_menu(
  Render_pipe&, 
  int active_button, 
  int prev_button,
  Text_box&,
  Text_box&,
  Text_box&
);

GAME_STATES get_state_level_menu(int active_button);

GAME_STATES level_menu(Render_pipe& rp, UI& ui);

GAME_STATES game_function(
  GAME_STATES state, 
  Render_pipe&, 
  UI&
);


GAME_STATES process_menu(
  Render_pipe&, 
  UI&
);

GAME_STATES process_help(
  Render_pipe&,
  UI&
);

GAME_STATES win_page(Render_pipe& rp, UI& ui);

GAME_STATES lose_page(Render_pipe& rp, UI& ui);

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

void process_help_key(
  SDL_Event&,
  GAME_STATES&,
  int& active_button
);

void change_active_button(
  Render_pipe&, 
  int active_button, 
  int prev_button,
  Text_box&,
  Text_box&,
  Text_box&
);

GAME_STATES get_state(int active_button);

GAME_STATES get_state_help(int active_button);

void add_life(
  Planet& pl, 
  Ship& sd
);

bool game_is_running(
  const Ship&, 
  const Planet&
);

#endif /* GAME_PROCESS_H_ */
