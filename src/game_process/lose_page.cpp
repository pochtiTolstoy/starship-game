#include "lose_page.h"

//==LOSE=PAGE===============================================================
GAME_STATES lose_page(Render_pipe& rp, UI& ui) {
  //Clear screen
  SDL_SetRenderDrawColor(rp.get_renderer(), 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(rp.get_renderer());
  //
  ui.reset_image_background(UI::BACKGROUND::MENU_BACK);

  const int offset = 70;
  
  Text_box lose_text(
    rp, "YOU LOSE...", 
    color_main_pass, color_shadow_pass, 4, true, 2
  );
  lose_text.set_position(
    (SCREEN_WIDTH - lose_text.get_width()) / 2,
    (SCREEN_HEIGHT - lose_text.get_height()) / 2 - offset
  );

  Text_box try_text(
    rp, "TRY AGAIN!",
    color_main_pass, color_shadow_pass, 4, true, 2
  );
  try_text.set_position(
    (SCREEN_WIDTH - try_text.get_width()) / 2,
    (SCREEN_HEIGHT - try_text.get_height()) / 2
  );

  //Quit button
  Text_box quit_button(
    rp, "QUIT", color_main_pass, color_shadow_pass, 4, false, 1
  );
  quit_button.set_position(
    (SCREEN_WIDTH - quit_button.get_width()) / 2,
    (SCREEN_HEIGHT - quit_button.get_height()) / 2 + offset
  );

  SDL_Event e;
  int prev_button = -1;
  int active_button = -1;
  GAME_STATES state = GAME_STATES::WIN;

  //For frame capping
  LTimer capTimer;
  int frameTicks = 0;


  while (state == GAME_STATES::WIN) {
    capTimer.start();
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) state = GAME_STATES::QUIT;
      else process_lose_key(e, state, active_button);
    }
    //Clear screen
    SDL_SetRenderDrawColor(rp.get_renderer(), 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(rp.get_renderer());

    //Render menu background
    ui.render_background(rp);

    //BAD
    if (prev_button != active_button) {
      switch (active_button) {
        case 0:
          quit_button.update_text(rp, "QUIT", color_main_act, color_shadow_act);
          break;
        default:
          std::cout << "There is no such button id: " << active_button << '\n';
          exit(EXIT_FAILURE);
      }
      switch (prev_button) {
        case -1: break;
        case 0:
          quit_button.update_text(rp, "QUIT", color_main_pass, color_shadow_pass);
          break;
        default:
          std::cout << "There is no such button id: " << active_button << '\n';
          exit(EXIT_FAILURE);
      }
      prev_button = active_button;
    }

    lose_text.render(rp);

    //Render congratulation text
    try_text.render(rp);

    //Render quit button
    quit_button.render(rp);

    //FINAL RENDER
    SDL_RenderPresent(rp.get_renderer());

    //Wait
    frameTicks = capTimer.getTicks();
    if (frameTicks < SCREEN_TICK_PER_FRAME) {
      SDL_Delay(SCREEN_TICK_PER_FRAME - frameTicks);
    }
  }
  return state;
}

void process_lose_key(
  SDL_Event& e,
  GAME_STATES& state,
  int& active_button
) {
  if (active_button == -1 && e.type == SDL_KEYDOWN) {
    active_button = 0;
    return;
  }
  if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
    switch (e.key.keysym.sym) {
      case SDLK_w:
      case SDLK_UP:
        active_button = (active_button == 0) ? 0 : --active_button;
        break;
      case SDLK_s:
      case SDLK_DOWN:
        active_button = (active_button == 0) ? 0 : ++active_button;
        break;
      case SDLK_RETURN:
        state = get_state_lose(active_button);
    }
  }
}

GAME_STATES get_state_lose(int active_button) {
  switch (active_button) {
    case 0: return GAME_STATES::MENU;
    default:
      std::cout << "Warning: there is no such button id: "
                << active_button << '\n';
      return GAME_STATES::QUIT;
  }
}
