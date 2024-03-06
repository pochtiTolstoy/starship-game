#include "util/constants.h"
#include "texture/LTexture.h"
#include "entity/game_controls.h"
#include "entity/ship.h"

bool init();
bool loadMedia();
void close();
bool process_key(SDL_Event&, Ship&, enemy enemy_arr[]);
void init_enemy(enemy& enemy_data, double angle);
void reinit_enemy(enemy&);
bool enemy_move(enemy& enemy_data, planet& pl);
int eu_mod(int num, int mod);
void shoot(Ship& sd, enemy enemy_arr[]);
void render_obj_health(obj_health& oh);
void render_killbar(ui_killbar&, const Ship&);
void detect_collision(Ship& ship_data, enemy meteor_arr[]);
void detect_collision_health(Ship& sd, obj_health& oh, planet& pl);
void add_life(planet& pl, Ship& sd);
void init_planet(planet&);
void calc_cooldown(Ship&);
void calc_spawn_health(obj_health& oh);
void init_color(SDL_Color& cl, int R, int G, int B);
bool game_is_running(const Ship&, const planet&);
int shoot_animation(const Ship&);
void spawn_health();
void init_obj_health(obj_health& oh);
void init_killbar(ui_killbar&);

//Things to get rid of
SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
TTF_Font* gFont = nullptr;

LTexture gEnemyTextures[NUM_ENEMY_TEXTURES];
LTexture gUITextures[NUM_UI_TEXTURES];
LTexture gBackground;
LTexture gTextTexture;

int main(int argc, char* args[]) {
  srand(time(0));
  //Render_pipe RP;
  if (!init()) {
    std::cout << "Failed to init!\n";
    close();
    exit(EXIT_FAILURE);
  }
  if (!loadMedia()) {
    std::cout << "Failed to load media!\n";
    close();
    exit(EXIT_FAILURE);
  }

  SDL_Event e;
  bool quit = false;

  Ship sd;
  planet pl;
  obj_health oh;
  ui_killbar uk;
  enemy meteor_arr[NUM_ENEMY_ON_MAP];

  init_planet(pl);
  init_obj_health(oh);
  init_killbar(uk);

  for (int i = 0; i < NUM_ENEMY_ON_MAP; ++i) {
    init_enemy(meteor_arr[i], 15 * i);
  }

  //Game loop
  while (!quit && game_is_running(sd, pl)) {
    while (SDL_PollEvent(&e) != 0) {
      quit = process_key(e, sd, meteor_arr);
    }
    //Clear screen
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    //Background
    gBackground.render(0, 0);

    //Hearts planet
    for (int i = 0; i < pl.curr_lifes; ++i) {
      gUITextures[0].render(
        SCREEN_WIDTH / 2 + (i - pl.max_lifes / 2) * gUITextures[0].get_width(),
        (SCREEN_HEIGHT - gUITextures[0].get_height()) / 2 + SHIFT_HEART_PLANET_Y
      );
    }
    for (int i = pl.curr_lifes; i < pl.max_lifes; ++i) {
      gUITextures[1].render(
        SCREEN_WIDTH / 2 + (i - pl.max_lifes / 2) * gUITextures[1].get_width(),
        (SCREEN_HEIGHT - gUITextures[1].get_height()) / 2 + SHIFT_HEART_PLANET_Y
      );
    }

    //Hearts ship
    for (int i = 0; i < sd.curr_lifes_; ++i) {
      gUITextures[0].render(
        SCREEN_WIDTH / 2 + (i - sd.max_lifes_ / 2) * gUITextures[0].get_width(),
        (SCREEN_HEIGHT - gUITextures[0].get_height()) / 2 + 
          SHIFT_HEART_PLANET_Y + SHIFT_HEART_SHIP_Y
      );
    }
    for (int i = sd.curr_lifes_; i < sd.max_lifes_; ++i) {
      gUITextures[1].render(
        SCREEN_WIDTH / 2 + (i - sd.max_lifes_ / 2) * gUITextures[1].get_width(),
        (SCREEN_HEIGHT - gUITextures[1].get_height()) / 2 + 
          SHIFT_HEART_PLANET_Y + SHIFT_HEART_SHIP_Y
      );
    }

    //Bullets
    for (int i = 0; i < sd.curr_bullets_; ++i) {
      gUITextures[2].render(
        SCREEN_WIDTH / 2 + (i - sd.max_bullets_ / 2) *
        gUITextures[2].get_width(),
        (SCREEN_HEIGHT -  gUITextures[2].get_height()) / 2 +
          SHIFT_HEART_PLANET_Y + 2 * SHIFT_HEART_SHIP_Y
      );
    }

    for (int i = sd.curr_bullets_; i < sd.max_bullets_; ++i) {
      gUITextures[3].render(
        SCREEN_WIDTH / 2 + (i - sd.max_bullets_ / 2) *
        gUITextures[3].get_width(),
        (SCREEN_HEIGHT -  gUITextures[3].get_height()) / 2 +
          SHIFT_HEART_PLANET_Y + 2 * SHIFT_HEART_SHIP_Y
      );
    }

    //Draw obj_health
    render_obj_health(oh);

    //Draw ship
    sd.render();

    //Draw enemy
    for (int i = 0; i < NUM_ENEMY_ON_MAP; ++i) {
      if (enemy_move(meteor_arr[i], pl)) {
        gEnemyTextures[0].render(
          meteor_arr[i].x_pos, meteor_arr[i].y_pos,
          nullptr, meteor_arr[i].rd
        );
      }
    }

    //Render text
    render_killbar(uk, sd);

    //Process final render
    SDL_RenderPresent(gRenderer);
    
    //Calculate game events
    detect_collision(sd, meteor_arr);
    calc_cooldown(sd);
    calc_spawn_health(oh);
    detect_collision_health(sd, oh, pl);
  }
  close();
  return 0;
}

bool process_key(SDL_Event& e, Ship& sd, enemy enemy_arr[]) {
  static const int MOVE_LEN = 30;
  if (e.type == SDL_QUIT) return true;
  if (e.type == SDL_KEYDOWN) {
    //Change frame
    switch(e.key.keysym.sym) {
      case SDLK_w: sd.image_ = Ship::STATES::MOVE_FORWARD; break;
      case SDLK_s: sd.image_ = Ship::STATES::MOVE_BACKWARD; break;
      case SDLK_SPACE: sd.image_ = shoot_animation(sd); break;
      case SDLK_a: 
        sd.image_ = (sd.image_ == Ship::STATES::RELOAD) ? 
                    Ship::STATES::RELOAD : Ship::STATES::DEFAULT;
        break;
      case SDLK_d:
        sd.image_ = (sd.image_ == Ship::STATES::RELOAD) ? 
                    Ship::STATES::RELOAD : Ship::STATES::DEFAULT;
        break;
      default:
        sd.image_ = Ship::STATES::DEFAULT;
    }

    //Change move
    switch(e.key.keysym.sym) {
      case SDLK_a: sd.render_.angle -= MOVE_ANGULAR;
        break;
      case SDLK_d: sd.render_.angle += MOVE_ANGULAR;
        break;
      case SDLK_w: 
        sd.y_pos_ -= MOVE_LEN;
        sd.render_.center.y += MOVE_LEN;
        break;
      case SDLK_s: 
        sd.y_pos_ += MOVE_LEN;
        sd.render_.center.y -= MOVE_LEN;
        break;
      case SDLK_e:
        sd.render_.angle += DEGREES_IN_HALF_CIRCLE;
        break;
      case SDLK_SPACE: 
        shoot(sd, enemy_arr);
        break;
    }
  }
  return false;
}

bool init() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "SDL could not init! SDL error: "
      << SDL_GetError() << '\n';
    return false;
  }
  if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
    std::cout << "Warning: Linear texture filtering not enabled!\n";
  }
  gWindow = SDL_CreateWindow(
    "SDL Tutorial",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN
  );
  if (gWindow == nullptr) {
    std::cout << "Window could not be created! SDL Error: "
      << SDL_GetError() << '\n';
    return false; 
  }
  gRenderer = SDL_CreateRenderer(
    gWindow, -1, SDL_RENDERER_ACCELERATED |
    SDL_RENDERER_PRESENTVSYNC
  );
  if (gRenderer == nullptr) {
    std::cout << "Renderer could not be crated! SDL Error: "
      << SDL_GetError() << '\n';
    return false;
  }
  SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
  int imgFlags = IMG_INIT_PNG;
  if (!(IMG_Init(imgFlags) & imgFlags)) {
    std::cout << "SDL_image could not init! SDL_image Error: "
      << IMG_GetError() << '\n';
    return false;
  }

  //Initialize SDL_ttf
  if (TTF_Init() == -1) {
    std::cout << "SDL_ttf could not init! SDL_ttf Error: "
      << TTF_GetError() << '\n';
    return false;
  }
  return true;
}

bool loadMedia() {
  for (int i = 0; i < NUM_ENEMY_TEXTURES; ++i) {
    if (!gEnemyTextures[i].loadFromFile(FILE_PATHS_ENEMY[i])) {
      std::cout << "Failed to load enemy texture!\n";
      return false;
    }
  }
  for (int i = 0; i < NUM_UI_TEXTURES; ++i) {
    if (!gUITextures[i].loadFromFile(FILE_PATHS_UI[i])) {
      std::cout << "Filed to load UI textures!\n";
      return false;
    }
  }
  if (!gBackground.loadFromFile(FILE_PATH_BACKGROUND)) {
    std::cout << "Failed to load planet!\n";
    return false;
  }

  //Open the font
  gFont = TTF_OpenFont(FILE_PATH_FONT, 35);
  if (nullptr == gFont) {
    std::cout << "Failed to load font! SDL_ttf Error: "
      << TTF_GetError() << '\n';
    return false;
  }
  return true;
}

void close() {
  gTextTexture.free();
  TTF_CloseFont(gFont);
  gFont = nullptr;
  for (int i = 0; i < NUM_ENEMY_TEXTURES; ++i) {
    gEnemyTextures[i].free();
  }
  for (int i = 0; i < NUM_UI_TEXTURES; ++i) {
    gUITextures[i].free();
  }
  gBackground.free();


  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gWindow);
  gWindow = nullptr;
  gRenderer = nullptr;

  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

void init_enemy(enemy& enemy_data, double angle) {
  enemy_data.w = gEnemyTextures[0].get_width();
  enemy_data.h = gEnemyTextures[0].get_height();
  enemy_data.x_pos = SPAWN_ENEMY_X;
  enemy_data.y_pos = (SCREEN_HEIGHT - enemy_data.h) / 2;
  enemy_data.shift_enemy = rand() % ENEMY_SPEED_LEVELS + 1;
  enemy_data.frame_rate = rand() % ENEMY_FRAMERATE_LEVELS + 1;
  enemy_data.current_frame = 0;
  enemy_data.rd.angle = angle;
  enemy_data.rd.center = {SCREEN_WIDTH / 2 - enemy_data.x_pos, enemy_data.h / 2};
  enemy_data.rd.flip = SDL_FLIP_NONE;
  enemy_data.draw = false;
  enemy_data.first_spawn = true;
}

void reinit_enemy(enemy& enemy_data) {
  enemy_data.x_pos = SPAWN_ENEMY_X;
  enemy_data.y_pos = (SCREEN_HEIGHT - enemy_data.h) / 2;
  enemy_data.current_frame = 0;
  enemy_data.rd.center = {SCREEN_WIDTH / 2 - enemy_data.x_pos, enemy_data.h / 2};
  enemy_data.draw = false;
  enemy_data.first_spawn = false;
}

void init_obj_health(obj_health& oh) {
  oh.w = gUITextures[0].get_width();
  oh.h = gUITextures[0].get_height();
  oh.x_pos = SPAWN_ENEMY_X;
  oh.y_pos = (SCREEN_HEIGHT - oh.h) / 2;
  oh.rd.angle = 0;
  oh.rd.center = {SCREEN_WIDTH / 2 - oh.x_pos, oh.h / 2};
  oh.rd.flip = SDL_FLIP_NONE;
  oh.draw = false;
}

void init_planet(planet& p) {
  p.max_lifes = 4;
  p.curr_lifes = 4;
}

void calc_spawn_health(obj_health& oh) {
  if (!oh.draw && rand() % 100 == 0) {
    // turn off after collision with ship
    oh.draw = true;
    oh.rd.angle = (rand() % 24) * 15;
    if ((oh.rd.angle >= 45 && oh.rd.angle <= 90 + 45) ||
         (oh.rd.angle >= 180 + 45 && oh.rd.angle <= 270 + 45)) {
      oh.x_pos = rand() % 200 + 20 + (SCREEN_WIDTH - SCREEN_HEIGHT) / 2;
    } else {
      oh.x_pos = rand() % 500 + 50; 
    }
    oh.rd.center = {SCREEN_WIDTH / 2 - oh.x_pos, oh.h / 2};
  }
}

void render_obj_health(obj_health& oh) {
  if (!oh.draw) return;
  gUITextures[0].render(
    oh.x_pos, oh.y_pos,
    nullptr, oh.rd
  );
}

bool enemy_move(enemy& ed, planet& p) {
  static const int planet_hitbox = 240;
  if (std::abs(SCREEN_WIDTH / 2 - ed.x_pos - ed.w / 4) <= planet_hitbox &&
    std::abs(SCREEN_HEIGHT / 2 - ed.y_pos - ed.h / 4) <= planet_hitbox) {
    reinit_enemy(ed); //reinit enemy
    --p.curr_lifes;
    return false;
  }
  int spawn_chance = ed.first_spawn ? RAND_SPAWN_FIRST : RAND_SPAWN;
  if (!ed.draw && rand() % spawn_chance == 0) {
    ed.draw = true;
    return false;
  }
  if (ed.draw == true && ed.current_frame % ed.frame_rate == 0) {
    ed.x_pos += ed.shift_enemy;
    ed.rd.center.x -= ed.shift_enemy;
  }
  ++ed.current_frame;
  return true;
}

void shoot(Ship& sd, enemy enemy_arr[]) {
  if (sd.curr_bullets_ <= 0) return;
  --sd.curr_bullets_;
  for (int i = 0; i < NUM_ENEMY_ON_MAP; ++i) {
    if (enemy_arr[i].draw && 
      (eu_mod(static_cast<int>(sd.render_.angle) + COORDS_SYNC, DEGREES_IN_CIRCLE)) == enemy_arr[i].rd.angle) {
      reinit_enemy(enemy_arr[i]);
      ++sd.kills_;
    }
  }
}

int eu_mod(int num, int mod) {
  int r = num % mod;
  if (r < 0) r += mod;
  return r;
}

void detect_collision(Ship& sd, enemy ma[]) {
  //Wrong, should be 1/2
  int wh_diff = SCREEN_WIDTH - SCREEN_HEIGHT;
  int spawn_diff = -SPAWN_ENEMY_X;
  int coords_sync = spawn_diff - wh_diff;
  int angle_sync = sd.render_.angle + COORDS_SYNC;

  for (int i = 0; i < NUM_ENEMY_ON_MAP; ++i) {
    if (sd.y_pos_ <= SCREEN_HEIGHT / 2) {
      if (eu_mod(angle_sync, 360) == eu_mod(ma[i].rd.angle, 360)) {
        if (std::abs(sd.y_pos_ - (ma[i].x_pos + coords_sync)) <= SHIP_HITBOX) {
          reinit_enemy(ma[i]);
          --sd.curr_lifes_;
        }
      }
    } else {
      if (eu_mod(angle_sync, 360) != eu_mod(ma[i].rd.angle, 360) &&
          eu_mod(angle_sync, 180) == eu_mod(ma[i].rd.angle, 180)) {
        if (std::abs(-sd.y_pos_ - sd.height_ + SCREEN_HEIGHT - (ma[i].x_pos + coords_sync)) <=
            SHIP_HITBOX - 30) {
          reinit_enemy(ma[i]);
          --sd.curr_lifes_;
        }
      }
    }
  }
}

void detect_collision_health(Ship& sd, obj_health& oh, planet& pl) {
  if (!oh.draw) return;
  int wh_diff = (SCREEN_WIDTH - SCREEN_HEIGHT) / 2;
  int spawn_diff = 0;
  int coords_sync = spawn_diff - wh_diff;
  int angle_sync = sd.render_.angle + COORDS_SYNC;
  int mid_ship_y = sd.y_pos_ + sd.height_ / 2 - 40;
  int mid_health_x = oh.x_pos + oh.w / 2;
  if (sd.y_pos_ <= SCREEN_HEIGHT / 2) {
    if (eu_mod(angle_sync, 360) == eu_mod(oh.rd.angle, 360)) {
      if (std::abs(mid_ship_y - mid_health_x - coords_sync) <= 25) {
        init_obj_health(oh);
        add_life(pl, sd);
      }
    }
    return;
  }
  if (eu_mod(angle_sync, 360) != eu_mod(oh.rd.angle, 360) &&
    eu_mod(angle_sync, 180) == eu_mod(oh.rd.angle, 180)) {
    if (std::abs(-sd.y_pos_ - sd.height_ / 2 + SCREEN_HEIGHT - mid_health_x - coords_sync)
    <= 30) {
      init_obj_health(oh);
      add_life(pl, sd);
    }
  }
}

void add_life(planet& pl, Ship& sd) {
  if (pl.curr_lifes < pl.max_lifes) ++pl.curr_lifes;
  else if (sd.curr_lifes_ < sd.max_lifes_) ++sd.curr_lifes_;
}

int shoot_animation(const Ship& sd) {
  return (sd.curr_bullets_ <= 0) ? Ship::STATES::RELOAD : Ship::STATES::SHOOT;
}

void calc_cooldown(Ship& sd) { 
  if (sd.curr_bullets_ > 0) return;
  ++sd.cooldown_timer_;
  if (sd.cooldown_timer_ >= sd.cooldown_) {
    sd.cooldown_timer_ = 0;
    sd.curr_bullets_ = sd.max_bullets_;
  }
}

void init_color(SDL_Color& cl, int r, int g, int b) {
  cl.r = r;
  cl.g = g;
  cl.b = b;
}

void init_killbar(ui_killbar& uk) {
  uk.max_kills = KILLS_TO_WIN;
  uk.curr_kills = 0;
  uk.text = "Kills: 0/" + std::to_string(uk.max_kills);
  uk.color = { 0, 0, 0 };
  gTextTexture.loadFromRenderedText(uk.text, uk.color);
}

void render_killbar(ui_killbar& uk, const Ship& sd) {
  if (uk.curr_kills != sd.kills_) {
    uk.curr_kills = sd.kills_;
    uk.text = "Kills: " + std::to_string(uk.curr_kills) +
      "/" + std::to_string(KILLS_TO_WIN);
    gTextTexture.loadFromRenderedText(uk.text, uk.color);
  }
  gTextTexture.render(
    (SCREEN_WIDTH - gTextTexture.get_width()) / 2,
    SCREEN_HEIGHT - gTextTexture.get_height()
  );
}

bool game_is_running(const Ship& sd, const planet& pl) {
  return sd.is_fighting() && pl.curr_lifes;
}
