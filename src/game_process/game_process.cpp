#include "game_process.h"

GAME_STATES game_function(
  GAME_STATES state, 
  Render_pipe& rp,
  UI& ui
) {
  switch (state) {
    //case GAME_STATES::PLAY: state = process_gameplay(rp, ui); break;
    case GAME_STATES::PLAY_MENU: state = level_menu(rp, ui); break;
    case GAME_STATES::HELP: state = process_help(rp, ui); break;
    case GAME_STATES::MENU: state = process_menu(rp, ui); break;
    case GAME_STATES::WIN:  state = win_page(rp, ui); break;
    case GAME_STATES::LVL1_LOSE:     
    case GAME_STATES::LVL2_LOSE: state = lose_page(rp, ui, state); break;
    case GAME_STATES::LVL1: state = process_gameplay1(rp, ui); break;
    case GAME_STATES::LVL2: state = process_gameplay2(rp, ui); break;
    default:
      std::cout << "Warning: there is no such game state as: " << state << '\n';
  }
  return state; //CODE FOR QUIT
}
