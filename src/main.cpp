#include "util/render_pipe.h"
#include "entity/ui.h"
#include "game_process/game_process.h"

int main() {
  //Render pipeline with SDL subsystems
  Render_pipe rp;
  if (!rp.init()) {
    std::cout << "Failed to init!\n";
    exit(EXIT_FAILURE);
  }

  //Class UI with ui textures and draw methods
  UI ui(rp);

  int state = 0;
  while (state != -1) {
    state = game_function(state, rp, ui);
  }
  return 0;
}
