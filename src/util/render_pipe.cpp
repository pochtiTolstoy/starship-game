#include "render_pipe.h"

Render_pipe::Render_pipe()
  :gWindow_(nullptr), gRenderer_(nullptr), gFont_(nullptr) {}
  
bool Render_pipe::init() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "SDL could not init! SDL error: "
      << SDL_GetError() << '\n';
    return false;
  }
  if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY), "1") {
    std::cout << "Warning: Linear texture filtering not enabled!\n";
  }
  gWindow_ = SDL_CreateWindow(
    "SDL Tutorial",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN
  );
  if (nullptr == gWindow_) {
    std::cout << "Window could not be created! SDL Error: "
      << SDL_GetError() << '\n';
    return false; 
  }
  gRenderer_ = SDL_CreateRenderer(
    gWindow_, -1, SDL_RENDERER_ACCELERATED |
    SDL_RENDERER_PRESENTVSYNC
  );
  if (nullptr == gRenderer_) {
    std::cout << "Renderer could not be crated! SDL Error: "
      << SDL_GetError() << '\n';
    return false;
  }
  SDL_SetRenderDrawColor(gRenderer_, 0xFF, 0xFF, 0xFF, 0xFF);
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
