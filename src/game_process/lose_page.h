#ifndef LOSE_PAGE_H_
#define LOSE_PAGE_H_

#include "../util/constants.h"
#include "../util/render_pipe.h"
#include "../util/timer.h"
#include "../util/util.h"

#include "../entity/level_images.h"
#include "../entity/text_box.h"
#include "../entity/ui.h"

GAME_STATES lose_page(Render_pipe &rp, UI &ui, GAME_STATES curr_lvl);
void process_lose_key(SDL_Event &e, GAME_STATES &state, int &active_button);
GAME_STATES get_state_lose(int active_button);

#endif /* LOSE_PAGE_H_ */
