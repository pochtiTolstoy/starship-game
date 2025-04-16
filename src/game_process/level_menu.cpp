#include "level_menu.h"

//==LEVEL=MENU================================================================
GAME_STATES level_menu(Render_pipe &rp, UI &ui) {
  // Clear screen
  SDL_SetRenderDrawColor(rp.get_renderer(), 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(rp.get_renderer());
  ui.reset_image_background(UI::BACKGROUND::MENU_BACK);
  GAME_STATES state = GAME_STATES::PLAY_MENU;
  int offset = 60;

  // Create level images
  Level_images level_images_bar(rp);
  level_images_bar.set_position(
      (SCREEN_WIDTH - level_images_bar.get_width()) / 2, 100);

  // Lvl button 1
  Text_box lvl_button1(rp, "LEVEL 1", color_main_pass, color_shadow_pass, 4,
                       false, 1);
  lvl_button1.set_position((SCREEN_WIDTH - lvl_button1.get_width()) / 2,
                           level_images_bar.get_bottom_y(0) + 15);

  // Lvl button 2
  Text_box lvl_button2(rp, "LEVEL 2", color_main_pass, color_shadow_pass, 4,
                       false, 1);
  lvl_button2.set_position((SCREEN_WIDTH - lvl_button2.get_width()) / 2,
                           level_images_bar.get_bottom_y(1) + 15);

  // Quit button
  Text_box quit_button(rp, "QUIT", color_main_pass, color_shadow_pass, 4, false,
                       1);
  quit_button.set_position((SCREEN_WIDTH - quit_button.get_width()) / 2,
                           (SCREEN_HEIGHT - quit_button.get_height()) - offset);

  SDL_Event e;
  int prev_button = -1;
  int active_button = -1;

  // For frame capping
  LTimer capTimer;
  int frameTicks = 0;

  while (state == GAME_STATES::PLAY_MENU) {
    capTimer.start();
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT)
        state = GAME_STATES::QUIT;
      else
        process_level_menu_key(e, state, active_button);
    }
    // Clear screen
    SDL_SetRenderDrawColor(rp.get_renderer(), 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(rp.get_renderer());

    // Render menu background
    ui.render_background(rp);

    level_images_bar.render(rp);

    if (prev_button != active_button) {
      change_active_button_level_menu(rp, active_button, prev_button,
                                      lvl_button1, lvl_button2, quit_button);
      prev_button = active_button;
    }

    // RENDER LVL BUTTON 1
    lvl_button1.render(rp);

    // RENDER LVL BUTTON 2
    lvl_button2.render(rp);

    // RENDER QUIT BUTTON
    quit_button.render(rp);

    // FINAL RENDER
    SDL_RenderPresent(rp.get_renderer());

    // Wait
    frameTicks = capTimer.getTicks();
    if (frameTicks < SCREEN_TICK_PER_FRAME) {
      SDL_Delay(SCREEN_TICK_PER_FRAME - frameTicks);
    }
  }
  return state;
}

void change_active_button_level_menu(Render_pipe &rp, int active_button,
                                     int prev_button, Text_box &lvl_button1,
                                     Text_box &lvl_button2,
                                     Text_box &quit_button) {
  // Change color of active button
  switch (active_button) {
  case 0:
    lvl_button1.update_text(rp, "LEVEL 1", color_main_act, color_shadow_act);
    break;
  case 1:
    lvl_button2.update_text(rp, "LEVEL 2", color_main_act, color_shadow_act);
    break;
  case 2:
    quit_button.update_text(rp, "QUIT", color_main_act, color_shadow_act);
    break;
  default:
    std::cout << "There is no such button id: " << active_button << '\n';
    exit(EXIT_FAILURE);
  }

  // Change color of passive button
  switch (prev_button) {
  case 0:
    lvl_button1.update_text(rp, "LEVEL 1", color_main_pass, color_shadow_pass);
    break;
  case 1:
    lvl_button2.update_text(rp, "LEVEL 2", color_main_pass, color_shadow_pass);
    break;
  case -1:
  case 2:
    quit_button.update_text(rp, "QUIT", color_main_pass, color_shadow_pass);
    break;
  default:
    std::cout << "There is no such button id: " << prev_button << '\n';
    exit(EXIT_FAILURE);
  }
}

void process_level_menu_key(SDL_Event &e, GAME_STATES &state,
                            int &active_button) {
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
      active_button = (active_button == 2) ? 2 : ++active_button;
      break;
    case SDLK_RETURN:
      // change
      state = get_state_level_menu(active_button);
    }
  }
}

GAME_STATES get_state_level_menu(int active_button) {
  switch (active_button) {
  case 0:
    return GAME_STATES::LVL1;
    break;
  case 1:
    return GAME_STATES::LVL2;
    break;
  case 2:
    return GAME_STATES::MENU;
    break;
  default:
    std::cout << "Warning: there is no such button id: " << active_button
              << '\n';
    return GAME_STATES::QUIT;
  }
}
