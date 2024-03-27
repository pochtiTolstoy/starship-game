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
#include "level_menu.h"
#include "menu.h"
#include "win_page.h"
#include "lose_page.h"

GAME_STATES game_function(
  GAME_STATES state, 
  Render_pipe&, 
  UI&
);

GAME_STATES process_help(
  Render_pipe&,
  UI&
);

GAME_STATES win_page(Render_pipe& rp, UI& ui);

GAME_STATES lose_page(Render_pipe& rp, UI& ui);

void process_help_key(
  SDL_Event&,
  GAME_STATES&,
  int& active_button
);

GAME_STATES get_state_help(int active_button);

#endif /* GAME_PROCESS_H_ */
