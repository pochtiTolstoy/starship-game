#include "game_process.h"

GAME_STATES game_function(
  GAME_STATES state, 
  Render_pipe& rp,
  UI& ui
) {
  switch (state) {
    case GAME_STATES::PLAY: state = process_gameplay(rp, ui); break;
    case GAME_STATES::HELP: state = process_help(rp, ui); break;
    case GAME_STATES::MENU: state = process_menu(rp, ui); break;
    case GAME_STATES::WIN:  state = win_page(rp, ui); break;
    case GAME_STATES::LOSE: state = lose_page(rp, ui); break;
    default:
      std::cout << "Warning: there is no such game state as: " << state << '\n';
  }
  return state; //CODE FOR QUIT
}

//==MENU=======================================================================
GAME_STATES process_menu(Render_pipe& rp, UI& ui) {
  //Clear screen
  SDL_SetRenderDrawColor(rp.get_renderer(), 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(rp.get_renderer());
  //
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
      change_active_button(
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
        state = get_state(active_button); 
        break;
    }
  }
}

GAME_STATES get_state(int active_button) {
  switch (active_button) {
    case 0: return GAME_STATES::PLAY;
    case 1: return GAME_STATES::HELP;
    case 2: return GAME_STATES::QUIT;
    default: 
      std::cout << "Warning: there is no such button id: "
                << active_button << '\n';
      return GAME_STATES::QUIT;
  }
}

GAME_STATES get_state_help(int active_button) {
  switch (active_button) {
    case 0: return GAME_STATES::MENU;
    default:
      std::cout << "Warning: there is no such button id: "
                << active_button << '\n';
      return GAME_STATES::QUIT;
  }
}

void change_active_button(
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

//==GAME========================================================================
GAME_STATES process_gameplay(Render_pipe& rp, UI& ui) {
  SDL_SetRenderDrawColor(rp.get_renderer(), 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(rp.get_renderer());

  ui.reset_image_background(UI::BACKGROUND::GAME_BACK1);
  Ship sd(rp); //Need only in game loop
  Orbit orb(rp); //Need only in game loop
  orb.set_mines_texture(ui.get_mine_texture());
  Health_module hp_module(rp); //Need only in game loop
  Planet pl; //Need only in game loop
  Obj_health obj_health_arr[NUM_OBJ_HEALTH_ON_MAP] = {
    Obj_health(ui.get_ui_texture(UI::IMAGES::RED_HEART)),
    Obj_health(ui.get_ui_texture(UI::IMAGES::RED_HEART))
  };
  Obj_orbit obj_orb(ui.get_ui_texture(UI::IMAGES::ORBIT_ELEMENT));
  UI_killbar uk(rp);
  //LTexture gFPSTextTexture;

  //Array of enemies
  Enemy meteor_arr[NUM_ENEMY_ON_MAP];
  for (int i = 0; i < NUM_ENEMY_ON_MAP; ++i) {
    meteor_arr[i].set_angle(i * 15);
    meteor_arr[i].set_texture(ui.get_enemy_texture(UI::ENEMY::METEOR));
  }
  
  SDL_Event e; //Event only for game loop
  bool quit = false; //Game loop
  LTimer fpsTimer; //Game loop
  LTimer capTimer; //Game loop
  SDL_Color textColor = { 255, 255, 255, 255 };
  int countedFrames = 0;
  double last_frame_time = 0.0;
  double delta_time = 0.0;
  //std::stringstream timeText;

  //Game loop
  fpsTimer.start();
  while (!quit && game_is_running(sd, pl)) {
    capTimer.start();
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) quit = true;
      process_key(e, sd, meteor_arr, orb);
    }

    //Calculate average FPS
    float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.0f);
    if (avgFPS > 2'000'000) avgFPS = 0;

    /*
    timeText.str("");
    timeText << "FPS: " << avgFPS;
    if (!gFPSTextTexture.loadFromRenderedText(
      rp, timeText.str().c_str(), textColor
    )) {
      std::cout << "Unable to render FPS texture!\n";
    }
    */
    
    delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0;
    last_frame_time = SDL_GetTicks();

    //Calculate moving ship parameters
    sd.move(delta_time);
    orb.move(delta_time);

    //Clear screen
    SDL_SetRenderDrawColor(rp.get_renderer(), 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(rp.get_renderer());

    //Background
    ui.render_background(rp);

    //Hearts planet
    ui.render_planet_health(rp, pl);

    //Hearts ship
    ui.render_ship_health(rp, sd);

    //Bullets
    ui.render_ship_bullets(rp, sd);

    //Draw obj_health
    for (int i = 0; i < NUM_OBJ_HEALTH_ON_MAP; ++i) {
      obj_health_arr[i].render(rp);
    }

    //Draw mines
    orb.render_mines(rp);

    //Draw obj_orbit
    obj_orb.render(rp);
    
    //Draw Health module
    if (hp_module.render(rp, delta_time, obj_health_arr)) {
      add_life(pl, sd);
    }

    //Draw orbit module
    orb.render(rp);

    //Draw ship
    sd.render(rp);

    //Draw enemy
    for (int i = 0; i < NUM_ENEMY_ON_MAP; ++i) {
      if (meteor_arr[i].detect_planet_collision(pl)) pl.dec_lifes(); 
      if (meteor_arr[i].move(delta_time)) meteor_arr[i].render(rp);
    }

    //Render text
    uk.render(rp, sd);

    //PROCESS FINAL RENDER
    SDL_RenderPresent(rp.get_renderer());
    
    //Calculate game events
    sd.detect_collision(meteor_arr);
    orb.detect_collision(meteor_arr);
    orb.process_mines_collision(meteor_arr);
    orb.calc_drop_mine();
    sd.calc_cooldown();
    obj_orb.calc_spawn(sd, orb);
    if (obj_orb.detect_collision(sd)) {
      orb.reinit(obj_orb.get_y(), obj_orb.get_r_data());
    }
    for (int i = 0; i < NUM_OBJ_HEALTH_ON_MAP; ++i) {
      obj_health_arr[i].calc_spawn();
      if (obj_health_arr[i].detect_collision(sd)) add_life(pl, sd);
    }
    hp_module.calc_spawn(sd, pl, obj_health_arr, NUM_OBJ_HEALTH_ON_MAP);

    ++countedFrames;
    int frameTicks = capTimer.getTicks();
    if (frameTicks < SCREEN_TICK_PER_FRAME) {
      SDL_Delay(SCREEN_TICK_PER_FRAME - frameTicks);
    }
  }
  if (sd.kills_ >= KILLS_TO_WIN) return GAME_STATES::WIN;
  else return GAME_STATES::LOSE;
}

void process_key(SDL_Event& e, Ship& sd, Enemy* enemy_arr, Orbit& orb) {
  //CHANGE ANIMATION
  if (e.type == SDL_KEYDOWN) {
    switch(e.key.keysym.sym) {
      case SDLK_w: sd.image_ = STATES::MOVE_FORWARD; break;
      case SDLK_s: sd.image_ = STATES::MOVE_BACKWARD; break;
      case SDLK_SPACE: sd.change_shoot_animation(); break;
      case SDLK_a: 
        sd.image_ = (sd.image_ == STATES::RELOAD) ? STATES::RELOAD : STATES::DEFAULT;
        break;
      case SDLK_d:
        sd.image_ = (sd.image_ == STATES::RELOAD) ? STATES::RELOAD : STATES::DEFAULT;
        break;
      default:
        sd.image_ = STATES::DEFAULT;
    }
  } else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
    switch(e.key.keysym.sym) {
      case SDLK_SPACE: sd.image_ = (sd.image_ == STATES::RELOAD) ? STATES::RELOAD : STATES::DEFAULT;
    }
  }
  //KEY CONTROLS
  if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
    switch (e.key.keysym.sym) {
      //SHIP KEYS:
      case SDLK_w: 
        sd.vel_r_ -= MOVE_LEN; 
        sd.moving_r_ = true;
        break;
      case SDLK_s: 
        sd.vel_r_ += MOVE_LEN; 
        sd.moving_r_ = true;
        break;
      case SDLK_a: 
        sd.vel_ang_ -= MOVE_ANGULAR; 
        sd.moving_ang_ = true;
        break;
      case SDLK_d: 
        sd.vel_ang_ += MOVE_ANGULAR; 
        sd.moving_ang_ = true;
        break;
      case SDLK_e: sd.render_.angle += DEGREES_IN_HALF_CIRCLE; break;
      case SDLK_SPACE: 
        sd.process_shooting(enemy_arr); // CAN CHANGE IMAGE OF SHIP
        break;
      //ORBIT KEYS:
      case SDLK_m:
        orb.change_speed(100);
        break;
    }
  } else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
    sd.render_.angle = std::floor(sd.render_.angle);
    int rem = eu_mod(static_cast<int>(sd.render_.angle), 
                     static_cast<int>(MOVE_ANGULAR));
    switch (e.key.keysym.sym) {
      //SHIP KEYS UP:
      case SDLK_w: 
        sd.vel_r_ += MOVE_LEN; 
        sd.moving_r_ = false;
        break;
      case SDLK_s: 
        sd.vel_r_ -= MOVE_LEN; 
        sd.moving_r_ = false;
        break;
      case SDLK_a: 
        sd.vel_ang_ += MOVE_ANGULAR;
        if (rem > 9)
          //DAMPER CLOCKWISE
          sd.render_.angle += (MOVE_ANGULAR - rem);
        else
          sd.render_.angle -= rem;
        sd.moving_ang_ = false;
        break;
      case SDLK_d:
        sd.vel_ang_ -= MOVE_ANGULAR;
        if (rem > 5)
          sd.render_.angle += (MOVE_ANGULAR - rem);
        else
          //DAMPER COUNTER-CLOCKWISE
          sd.render_.angle -= rem;
        sd.moving_ang_ = false;
        break;
      //ORBIT KEYS UP:
      case SDLK_m:
        orb.change_speed(-100);
        break;
    }
  }
}

void add_life(Planet& pl, Ship& sd) {
  if (pl.get_curr_lifes() < pl.get_max_lifes()) pl.inc_lifes();
  else if (sd.curr_lifes_ < sd.max_lifes_) ++sd.curr_lifes_;
}

bool game_is_running(const Ship& sd, const Planet& pl) {
  return sd.is_fighting() && pl.get_curr_lifes();
}

//==HELP========================================================
GAME_STATES process_help(Render_pipe& rp, UI& ui) {
  SDL_SetRenderDrawColor(rp.get_renderer(), 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(rp.get_renderer());
  //
  ui.reset_image_background(UI::BACKGROUND::MENU_BACK);

  const int offset = 60;

  Text_box help_text(
    rp, HELP_TEXT, color_main_pass, color_shadow_pass, 2, true, 0
  );

  help_text.set_position(
    (SCREEN_WIDTH - help_text.get_width()) / 2,
    (SCREEN_HEIGHT - help_text.get_height()) / 2
  );

  Text_box quit_button(
    rp, "QUIT", color_main_pass, color_shadow_pass, 4, false, 1
  );

  quit_button.set_position(
    (SCREEN_WIDTH - quit_button.get_width()) / 2,
    (SCREEN_HEIGHT - quit_button.get_height()) - offset
  );

  GAME_STATES state = GAME_STATES::HELP;

  SDL_Event e;
  int prev_button = -1;
  int active_button = -1;

  //For frame capping
  LTimer capTimer;
  int frameTicks = 0;


  while (state == GAME_STATES::HELP) {
    capTimer.start();
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) state = GAME_STATES::QUIT;
      else process_help_key(e, state, active_button);
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

    //Render help text
    help_text.render(rp);

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

void process_help_key(
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
        state = get_state_help(active_button);
    }
  }
}

//==WIN=PAGE=================================================================
GAME_STATES win_page(Render_pipe& rp, UI& ui) {
  //Clear screen
  SDL_SetRenderDrawColor(rp.get_renderer(), 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(rp.get_renderer());
  //
  ui.reset_image_background(UI::BACKGROUND::MENU_BACK);

  const int offset = 70;
  
  Text_box win_text(
    rp, "YOU WON!", 
    color_main_pass, color_shadow_pass, 4, true, 2
  );
  win_text.set_position(
    (SCREEN_WIDTH - win_text.get_width()) / 2,
    (SCREEN_HEIGHT - win_text.get_height()) / 2 - offset
  );

  Text_box congr_text(
    rp, "CONGRATULATIONS!",
    color_main_pass, color_shadow_pass, 4, true, 2
  );
  congr_text.set_position(
    (SCREEN_WIDTH - congr_text.get_width()) / 2,
    (SCREEN_HEIGHT - congr_text.get_height()) / 2
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
      else process_help_key(e, state, active_button);
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

    //Render help text
    win_text.render(rp);

    //Render congratulation text
    congr_text.render(rp);

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
      else process_help_key(e, state, active_button);
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

    //Render help text
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
