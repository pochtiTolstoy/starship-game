#ifndef WIN_PAGE_H_
#define WIN_PAGE_H_

#include "../util/util.h"
#include "../util/render_pipe.h"
#include "../util/timer.h"
#include "../util/constants.h"

#include "../entity/ui.h"
#include "../entity/text_box.h"
#include "../entity/level_images.h"

GAME_STATES win_page(Render_pipe& rp, UI& ui);
void process_win_key(SDL_Event&, GAME_STATES&, int& active_button);
GAME_STATES get_state_win(int active_button);

#endif /* WIN_PAGE_H_ */
