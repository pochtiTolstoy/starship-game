#include "entity/ui.h"
#include "game_process/game_process.h"
#include "util/render_pipe.h"

int main() {
  srand(time(0));
  // Render pipeline with SDL subsystems
  Render_pipe rp;
  if (!rp.init()) {
    std::cout << "Failed to init!\n";
    exit(EXIT_FAILURE);
  }

  // Class UI with ui textures and draw methods
  UI ui(rp);

  GAME_STATES state = GAME_STATES::MENU;
  while (state != GAME_STATES::QUIT) {
    state = game_function(state, rp, ui);
  }
  return 0;
}
