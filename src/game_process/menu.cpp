#include "menu.h"

//==MENU=======================================================================
GAME_STATES process_menu(Render_pipe& rp, UI& ui) {
  //Clear screen
  SDL_SetRenderDrawColor(rp.get_renderer(), 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(rp.get_renderer());
  ui.reset_image_background(UI::BACKGROUND::MENU_BACK);
  const int offset = 70;

  //Play button
  Text_box play_button(
    rp, "PLAY", color_main_pass, color_shadow_pass, 4, false, 1
  );
  play_button.set_position(
    (SCREEN_WIDTH - play_button.get_width()) / 2,
    (SCREEN_HEIGHT - play_button.get_height()) / 2 - offset
  );

  //Help button
  Text_box help_button(
    rp, "HELP", color_main_pass, color_shadow_pass, 4, false, 1
  );
  help_button.set_position(
    (SCREEN_WIDTH - help_button.get_width()) / 2,
    (SCREEN_HEIGHT - help_button.get_height()) / 2
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
  GAME_STATES state = GAME_STATES::MENU;
  int prev_button = -1;
  int active_button = -1;

  //For frame capping
  LTimer capTimer;
  int frameTicks = 0;


  while (state == GAME_STATES::MENU) {
    capTimer.start(); //For SDL delay
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) state = GAME_STATES::QUIT;
      else process_menu_key(e, state, active_button);
    }
    //Clear screen
    SDL_SetRenderDrawColor(rp.get_renderer(), 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(rp.get_renderer());

    //Render menu background
    ui.render_background(rp);

    //Swap colors of buttons
    if (prev_button != active_button) {
      //std::cout << "SWAP COLORS, RERENDER.\n";
      change_active_button_menu(
        rp, active_button, prev_button, play_button, help_button, quit_button
      ); 
      prev_button = active_button;
    }

    //Render play button
    play_button.render(rp);

    //Render help button
    help_button.render(rp);

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

void process_menu_key(
  SDL_Event& e, 
  GAME_STATES& state, 
  int& active_button
) {
  if (active_button == -1 && e.type == SDL_KEYDOWN) {
    active_button = 0;
    return;
  }
  //Change after adding new buttons!
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
        state = get_menu_state(active_button); 
        break;
    }
  }
}

GAME_STATES get_menu_state(int active_button) {
  switch (active_button) {
    case 0: return GAME_STATES::PLAY_MENU;
    case 1: return GAME_STATES::HELP;
    case 2: return GAME_STATES::QUIT;
    default: 
      std::cout << "Warning: there is no such button id: "
                << active_button << '\n';
      return GAME_STATES::QUIT;
  }
}

void change_active_button_menu(
  Render_pipe& rp, 
  int active_button, 
  int prev_button,
  Text_box& play_button,
  Text_box& help_button,
  Text_box& quit_button
) {

  //Change color of active button
  switch (active_button) {
    case 0:
      play_button.update_text(rp, "PLAY", color_main_act, color_shadow_act);
      break;
    case 1:
      help_button.update_text(rp, "HELP", color_main_act, color_shadow_act); 
      break;
    case 2:
     quit_button.update_text(rp, "QUIT", color_main_act, color_shadow_act);
      break;
    default:
      std::cout << "There is no such button id: " << active_button << '\n';
      exit(EXIT_FAILURE);
  }

  //Change color of passive button
  switch (prev_button) {
    case 0: 
      play_button.update_text(rp, "PLAY", color_main_pass, color_shadow_pass);
      break;
    case 1:
      help_button.update_text(rp, "HELP", color_main_pass, color_shadow_pass); 
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
