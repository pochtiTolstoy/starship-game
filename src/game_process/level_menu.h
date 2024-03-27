#ifndef GAME_PAGES_H_
#define GAME_PGAES_H_

//UTIL HEADERS
#include "../util/util.h"
#include "../util/render_pipe.h"
#include "../util/timer.h"
#include "../util/constants.h"

//ENTITY HEADERS
#include "../entity/ui.h"
#include "../entity/text_box.h"
#include "../entity/level_images.h"

GAME_STATES level_menu(Render_pipe& rp, UI& ui);
void change_active_button_level_menu(
  Render_pipe& rp, 
  int active_button, 
  int prev_button,
  Text_box& lvl_button1,
  Text_box& lvl_button2,
  Text_box& quit_button
);
void process_level_menu_key(
  SDL_Event& e,
  GAME_STATES& state,
  int& active_button
);
GAME_STATES get_state_level_menu(int active_button);

#endif /* GAME_PAGES_H_ */
