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
bool process_key(SDL_Event&, game_controls&);
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
  int x_pos = (SCREEN_WIDTH - gShipTextures[0].getWidth()) / 2;
  int y_pos = (SCREEN_HEIGHT - gShipTextures[0].getHeight()) / 2;
  int width = gShipTextures[0].getWidth() / 2;
  int height = gShipTextures[0].getHeight() / 2;
  SDL_Point center = { width, height };
  game_controls ship_data = { 
    0, 0.0, 0.0, 
    width, height, 
    SDL_FLIP_NONE, &center
  };

  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      quit = process_key(e, ship_data);
    }
    //Set Render
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    //Draw ship
    gShipTextures[ship_data.frame].render(
      x_pos, y_pos - ship_data.shift_texture,
      nullptr, ship_data
    );

    //Draw enemy
    gEnemyTextures[0].render(
      400, 400, nullptr
    );

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

bool process_key(SDL_Event& e, game_controls& ar) {
  static const int MOVE_FORWARD = 30;
  static const double MOVE_ANGULAR = 15;
  if (e.type == SDL_QUIT) return true;
  if (e.type == SDL_KEYDOWN) {
    //Change move
    switch(e.key.keysym.sym) {
      case SDLK_a: ar.angle -= MOVE_ANGULAR;
        break;
      case SDLK_d: ar.angle += MOVE_ANGULAR;
        break;
      case SDLK_w: 
        ar.shift_texture += MOVE_FORWARD; 
        ar.center->y += MOVE_FORWARD;
        break;
      case SDLK_s: 
        ar.shift_texture -= MOVE_FORWARD; 
        ar.center->y -= MOVE_FORWARD;
        break;
    }

    //Change frame
    switch(e.key.keysym.sym) {
      case SDLK_w: ar.frame = 1; break;
      default:     ar.frame = 0;
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
