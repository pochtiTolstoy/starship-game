#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include "constants.h"
#include "LTexture.h"
#include "game_controls.h"

bool init();
bool loadMedia();
void close();
bool process_key(SDL_Event&, ship&);
void init_ship(ship& ship_data);
void init_enemy(enemy& enemy_data);
void enemy_move(enemy& enemy_data);
SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
LTexture gShipTextures[NUM_SHIP_TEXTURES];
LTexture gEnemyTextures[NUM_ENEMY_TEXTURES];

int main(int argc, char* args[]) {
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
  enemy meteor_data;
  init_ship(ship_data);
  init_enemy(meteor_data);

  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      quit = process_key(e, ship_data);
    }
    //Set Render
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    //Draw ship
    gShipTextures[ship_data.image].render(
      ship_data.x_pos, ship_data.y_pos - ship_data.shift_ship,
      nullptr, ship_data.rd
    );

    //Draw enemy
    gEnemyTextures[0].render(
      meteor_data.x_pos, meteor_data.y_pos,
      nullptr, meteor_data.rd
    );
    enemy_move(meteor_data);

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

bool process_key(SDL_Event& e, ship& sd) {
  static const int MOVE_FORWARD = 30;
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
        sd.shift_ship += MOVE_FORWARD; 
        sd.rd.center.y += MOVE_FORWARD;
        break;
      case SDLK_s: 
        sd.shift_ship -= MOVE_FORWARD; 
        sd.rd.center.y -= MOVE_FORWARD;
        break;
    }

    //Change frame
    switch(e.key.keysym.sym) {
      case SDLK_w: sd.image = 1; break;
      default:     sd.image = 0;
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
    "pics/ship1moveBig.png"
  };
  static const char* file_paths_enemy[NUM_ENEMY_TEXTURES] = {
    "pics/meteor1Big.png"
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
  return true;
}

void close() {
  for (int i = 0; i < NUM_SHIP_TEXTURES; ++i) {
    gShipTextures[i].free();
  }
  for (int i = 0; i < NUM_SHIP_TEXTURES; ++i) {
    gEnemyTextures[i].free();
  }

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
  ship_data.image = 0;
  ship_data.shift_ship = 0;
  ship_data.rd.angle = 0;
  ship_data.rd.center = {ship_data.w / 2, ship_data.h / 2};
  ship_data.rd.flip = SDL_FLIP_NONE;
}

void init_enemy(enemy& enemy_data) {
  enemy_data.w = gEnemyTextures[0].getWidth();
  enemy_data.h = gEnemyTextures[0].getHeight();
  enemy_data.x_pos = -256;
  enemy_data.y_pos = (SCREEN_HEIGHT - enemy_data.w) / 2;
  enemy_data.shift_enemy = 8;
  enemy_data.frame_rate = 16;
  enemy_data.current_frame = 0;
  enemy_data.rd.angle = 0;
  enemy_data.rd.center = {0, 0};
  enemy_data.rd.flip = SDL_FLIP_NONE;
}


void enemy_move(enemy& ed) {
  if (ed.current_frame % ed.frame_rate == 0) {
    ed.x_pos += ed.shift_enemy;
  }
  ++ed.current_frame;
}
