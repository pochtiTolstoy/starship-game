#ifndef HELP_H_
#define HELP_H_

#include "../util/util.h"
#include "../util/render_pipe.h"
#include "../util/timer.h"
#include "../util/constants.h"

#include "../entity/ui.h"
#include "../entity/text_box.h"
#include "../entity/level_images.h"

GAME_STATES process_help(
  Render_pipe&,
  UI&
);

void process_help_key(
  SDL_Event&,
  GAME_STATES&,
  int& active_button
);

GAME_STATES get_state_help(int active_button);

#endif /* HELP_H_ */
