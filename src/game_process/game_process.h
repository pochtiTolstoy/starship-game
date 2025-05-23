#ifndef GAME_PROCESS_H_
#define GAME_PROCESS_H_

#include <sstream>

// UTIL HEADERS
#include "../util/constants.h"

// GAME PAGES HEADERS
#include "game_levels.h"
#include "help.h"
#include "level_menu.h"
#include "lose_page.h"
#include "menu.h"
#include "win_page.h"

GAME_STATES game_function(GAME_STATES state, Render_pipe &, UI &);

#endif /* GAME_PROCESS_H_ */
