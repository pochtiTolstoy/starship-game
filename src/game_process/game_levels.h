#ifndef GAME_LEVELS_H_
#define GAME_LEVELS_H_

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

GAME_STATES process_gameplay1(
  Render_pipe&, 
  UI&
);

GAME_STATES process_gameplay2(
  Render_pipe&, 
  UI&
);

void process_key(SDL_Event& e, Ship& sd, Enemy* enemy_arr, Orbit& orb);
void process_help_key(SDL_Event& e, GAME_STATES& state, int& active_button);
GAME_STATES get_state_help(int active_button);
void add_life(Planet& pl, Ship& sd);
bool game_is_running(const Ship& sd, const Planet& pl);

#endif /* GAME_LEVELS_H_ */
