#ifndef MENU_H_
#define MENU_H_

// UTIL HEADERS
#include "../util/constants.h"
#include "../util/render_pipe.h"
#include "../util/timer.h"
#include "../util/util.h"

// ENTITY HEADERS
#include "../entity/text_box.h"
#include "../entity/ui.h"

GAME_STATES process_menu(Render_pipe &rp, UI &ui);
void process_menu_key(SDL_Event &e, GAME_STATES &state, int &active_button);
GAME_STATES get_menu_state(int active_button);
void change_active_button_menu(Render_pipe &, int active_button,
                               int prev_button, Text_box &, Text_box &,
                               Text_box &);

#endif /* MENU_H_ */
