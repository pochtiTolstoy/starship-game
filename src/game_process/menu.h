#ifndef MENU_H_
#define MENU_H_

//UTIL HEADERS
#include "../util/util.h"
#include "../util/render_pipe.h"
#include "../util/timer.h"
#include "../util/constants.h"

//ENTITY HEADERS
#include "../entity/ui.h"
#include "../entity/text_box.h"

GAME_STATES process_menu(Render_pipe& rp, UI& ui);
void process_menu_key(
  SDL_Event& e, 
  GAME_STATES& state, 
  int& active_button
);
GAME_STATES get_menu_state(int active_button);
void change_active_button_menu(
  Render_pipe&, 
  int active_button, 
  int prev_button,
  Text_box&,
  Text_box&,
  Text_box&
);

#endif /* MENU_H_ */
