#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "constants.h"
#include "LTexture.h"
#include "game_controls.h"

enum SHIP { DEFAULT, MOVE_FORWARD, SHOOT, MOVE_BACKWARD };

bool init();
bool loadMedia();
void close();
bool process_key(SDL_Event&, ship&, enemy enemy_arr[]);
void init_ship(ship& ship_data);
void init_enemy(enemy& enemy_data, double angle);
bool enemy_move(enemy& enemy_data, planet& pl);
int eu_mod(int num, int mod);
void shoot(double angle, enemy enemy_arr[]);
void render_ship(const ship& sd);
void detect_collision(ship& ship_data, enemy meteor_arr[]);
void init_planet(planet&);
SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
LTexture gShipTextures[NUM_SHIP_TEXTURES];
LTexture gEnemyTextures[NUM_ENEMY_TEXTURES];
LTexture gUITextures[NUM_UI_TEXTURES];
LTexture gBackground;

int main(int argc, char* args[]) {
  srand(time(0));
  if (!init()) {
    std::cout << "Failed to init!\n";
    exit(EXIT_FAILURE);
  }
  if (!loadMedia()) {
    std::cout << "Failed to load media!\n";
    exit(EXIT_FAILURE);
  }

  SDL_Event e;
  bool quit = false;
  ship ship_data;
  planet pl;
  enemy meteor_arr[NUM_ENEMY_ON_MAP];

  init_ship(ship_data);
  init_planet(pl);

  for (int i = 0; i < NUM_ENEMY_ON_MAP; ++i) {
    init_enemy(meteor_arr[i], 15 * i);
  }

  while (!quit && ship_data.curr_lifes && pl.curr_lifes) {
    while (SDL_PollEvent(&e) != 0) {
      quit = process_key(e, ship_data, meteor_arr);
    }
    //Set Render
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    //Background
    gBackground.render(0, 0);

    //Hearts planet
    for (int i = 0; i < pl.curr_lifes; ++i) {
      gUITextures[0].render(
        SCREEN_WIDTH / 2 + (i - 2) * gUITextures[0].getWidth(),
        (SCREEN_HEIGHT - gUITextures[0].getHeight()) / 2 + SHIFT_HEART_PLANET_Y
      );
    }
    for (int i = pl.curr_lifes; i < pl.max_lifes; ++i) {
      gUITextures[1].render(
        SCREEN_WIDTH / 2 + (i - 2) * gUITextures[1].getWidth(),
        (SCREEN_HEIGHT - gUITextures[1].getHeight()) / 2 + SHIFT_HEART_PLANET_Y
      );
    }
    //Hearts ship
    for (int i = 0; i < ship_data.curr_lifes; ++i) {
      gUITextures[0].render(
        SCREEN_WIDTH / 2 + (i - 1) * gUITextures[0].getWidth(),
        (SCREEN_HEIGHT - gUITextures[0].getHeight()) / 2 + 
          SHIFT_HEART_PLANET_Y + SHIFT_HEART_SHIP_Y
      );
    }
    for (int i = ship_data.curr_lifes; i < ship_data.max_lifes; ++i) {
      gUITextures[1].render(
        SCREEN_WIDTH / 2 + (i - 1) * gUITextures[1].getWidth(),
        (SCREEN_HEIGHT - gUITextures[1].getHeight()) / 2 + 
          SHIFT_HEART_PLANET_Y + SHIFT_HEART_SHIP_Y
      );
    }

    //Draw ship
    render_ship(ship_data);

    //Draw enemy
    for (int i = 0; i < NUM_ENEMY_ON_MAP; ++i) {
      if (enemy_move(meteor_arr[i], pl)) {
        gEnemyTextures[0].render(
          meteor_arr[i].x_pos, meteor_arr[i].y_pos,
          nullptr, meteor_arr[i].rd
        );
      }
    }
    detect_collision(ship_data, meteor_arr);
    //std::cout << "Lifes: " << ship_data.lifes << '\n';

    //Draw grid lines
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
    SDL_RenderDrawLine(
      gRenderer,
      0, SCREEN_HEIGHT / 2,
      SCREEN_WIDTH, SCREEN_HEIGHT / 2
    );
    SDL_RenderDrawLine(
      gRenderer,
      SCREEN_WIDTH / 2, 0,
      SCREEN_WIDTH / 2, SCREEN_HEIGHT
    );

    //Process final render
    SDL_RenderPresent(gRenderer);
  }
  close();
  return 0;
}

bool process_key(SDL_Event& e, ship& sd, enemy enemy_arr[]) {
  static const int MOVE_LEN = 30;
  static const double MOVE_ANGULAR = 15;
  if (e.type == SDL_QUIT) return true;
  if (e.type == SDL_KEYDOWN) {
    //Change move
    switch(e.key.keysym.sym) {
      case SDLK_a: sd.rd.angle -= MOVE_ANGULAR;
        break;
      case SDLK_d: sd.rd.angle += MOVE_ANGULAR;
        break;
      case SDLK_w: 
        sd.shift_ship += MOVE_LEN; 
        sd.rd.center.y += MOVE_LEN;
        break;
      case SDLK_s: 
        sd.shift_ship -= MOVE_LEN; 
        sd.rd.center.y -= MOVE_LEN;
        break;
      case SDLK_SPACE: 
        shoot(sd.rd.angle, enemy_arr);
        break;
    }

    //Change frame
    switch(e.key.keysym.sym) {
      case SDLK_w: sd.image = SHIP::MOVE_FORWARD; break;
      case SDLK_s: sd.image = SHIP::MOVE_BACKWARD; break;
      case SDLK_SPACE: sd.image = SHIP::SHOOT; break;
      default:     sd.image = SHIP::DEFAULT;
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
    SDL_WINDOW_SHOWN
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
  return true;
}

bool loadMedia() {
  static const char* file_paths_ship[NUM_SHIP_TEXTURES] = {
    "pics/ship1Big.png",
    "pics/ship1moveBig.png",
    "pics/ship_shoot1Big.png",
    "pics/ship_back1Big.png"
  };
  static const char* file_paths_enemy[NUM_ENEMY_TEXTURES] = {
    "pics/meteor1Big.png"
  };
  static const char* file_paths_ui[NUM_UI_TEXTURES] = {
    "pics/heartBig.png",
    "pics/heartBlackBig.png"
  };
  for (int i = 0; i < NUM_SHIP_TEXTURES; ++i) {
    if (!gShipTextures[i].loadFromFile(file_paths_ship[i])) {
      std::cout << "Failed to load ship texture!\n";
      return false;
    }
  }
  for (int i = 0; i < NUM_ENEMY_TEXTURES; ++i) {
    if (!gEnemyTextures[i].loadFromFile(file_paths_enemy[i])) {
      std::cout << "Failed to load enemy texture!\n";
      return false;
    }
  }
  for (int i = 0; i < NUM_UI_TEXTURES; ++i) {
    if (!gUITextures[i].loadFromFile(file_paths_ui[i])) {
      std::cout << "Filed to load UI textures!\n";
      return false;
    }
  }
  if (!gBackground.loadFromFile("pics/planet1Big.png")) {
    std::cout << "Failed to load planet!\n";
    return false;
  }
  return true;
}

void close() {
  for (int i = 0; i < NUM_SHIP_TEXTURES; ++i) {
    gShipTextures[i].free();
  }
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

  IMG_Quit();
  SDL_Quit();
}

void init_ship(ship& ship_data) {
  ship_data.w = gShipTextures[0].getWidth();
  ship_data.h = gShipTextures[0].getHeight();
  ship_data.x_pos = (SCREEN_WIDTH - ship_data.w) / 2;
  ship_data.y_pos = (SCREEN_HEIGHT - ship_data.h) / 2;
  ship_data.image = DEFAULT;
  ship_data.shift_ship = 0;
  ship_data.rd.angle = 0;
  ship_data.rd.center = {ship_data.w / 2, ship_data.h / 2};
  ship_data.rd.flip = SDL_FLIP_NONE;
  ship_data.max_lifes = 2;
  ship_data.curr_lifes = 2;
}

void init_enemy(enemy& enemy_data, double angle) {
  enemy_data.w = gEnemyTextures[0].getWidth();
  enemy_data.h = gEnemyTextures[0].getHeight();
  enemy_data.x_pos = SPAWN_ENEMY_X;
  enemy_data.y_pos = (SCREEN_HEIGHT - enemy_data.h) / 2;
  //enemy_data.x_pos = (SCREEN_WIDTH - enemy_data.w) / 2;
  //enemy_data.y_pos = -400;
  enemy_data.shift_enemy = rand() % 4 + 1;
  enemy_data.frame_rate = rand() % 8 + 1;
  enemy_data.current_frame = 0;
  enemy_data.rd.angle = angle;
  enemy_data.rd.center = {SCREEN_WIDTH / 2 - enemy_data.x_pos, enemy_data.h / 2};
  enemy_data.rd.flip = SDL_FLIP_NONE;
  enemy_data.draw = false;
}

void init_planet(planet& p) {
  p.max_lifes = 4;
  p.curr_lifes = 4;
}


bool enemy_move(enemy& ed, planet& p) {
  static const int planet_hitbox = 256;
  static const int RAND_SPAWN = 350;
  if (std::abs(SCREEN_WIDTH / 2 - ed.x_pos - ed.w / 4) <= planet_hitbox &&
    std::abs(SCREEN_HEIGHT / 2 - ed.y_pos - ed.h / 4) <= planet_hitbox) {
    init_enemy(ed, ed.rd.angle); //reinit enemy
    --p.curr_lifes;
    return false;
  }
  if (ed.draw == false && rand() % RAND_SPAWN == 0) {
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

void shoot(double angle, enemy enemy_arr[]) {
  for (int i = 0; i < NUM_ENEMY_ON_MAP; ++i) {
    if (enemy_arr[i].draw && 
      (eu_mod(static_cast<int>(angle) + COORDS_SYNC, DEGREES_IN_CIRCLE)) == enemy_arr[i].rd.angle) {
      init_enemy(enemy_arr[i], enemy_arr[i].rd.angle);
    }
  }
}

int eu_mod(int num, int mod) {
  int r = num % mod;
  if (r < 0) r += mod;
  return r;
}

void render_ship(const ship& sd) {
  int y_pos = sd.y_pos - sd.shift_ship;
  render_data rd = sd.rd;
  if (sd.image == SHOOT) {
    y_pos -= (gShipTextures[SHOOT].getHeight() - sd.h);
    rd.center.y = gShipTextures[SHOOT].getHeight() - sd.h / 2 + sd.shift_ship;
  }
  gShipTextures[sd.image].render(
    sd.x_pos, y_pos,
    nullptr, rd
  );
}

void detect_collision(ship& sd, enemy ma[]) {
  int y = sd.y_pos - sd.shift_ship;
  int wh_diff = SCREEN_WIDTH - SCREEN_HEIGHT;
  int spawn_diff = -SPAWN_ENEMY_X;
  int coords_sync = spawn_diff - wh_diff;
  int angle_sync = sd.rd.angle + COORDS_SYNC;

  for (int i = 0; i < NUM_ENEMY_ON_MAP; ++i) {
    if (y <= SCREEN_HEIGHT / 2) {
      if (eu_mod(angle_sync, 360) == eu_mod(ma[i].rd.angle, 360)) {
        if (std::abs(y - (ma[i].x_pos + coords_sync)) <= SHIP_HITBOX) {
          init_enemy(ma[i], ma[i].rd.angle);
          --sd.curr_lifes;
        }
      }
    } else {
      if (eu_mod(angle_sync, 360) != eu_mod(ma[i].rd.angle, 360) &&
          eu_mod(angle_sync, 180) == eu_mod(ma[i].rd.angle, 180)) {
        if (std::abs(y + 2*sd.shift_ship - (ma[i].x_pos + coords_sync)) <=
            SHIP_HITBOX - 30) {
          init_enemy(ma[i], ma[i].rd.angle);
          --sd.curr_lifes;
        }
      }
    }
  }
}

