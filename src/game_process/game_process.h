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

int game_function(
  int state, 
  Render_pipe&, 
  const UI&
);

void process_gameplay(
  Render_pipe&, 
  const UI&
);

void process_key(
  SDL_Event&, Ship&, 
  Enemy* enemy_arr, 
  Orbit& orb
);

void add_life(
  Planet& pl, 
  Ship& sd
);

bool game_is_running(
  const Ship&, 
  const Planet&
);

#endif /* GAME_PROCESS_H_ */
