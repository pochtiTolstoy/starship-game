#include "render_pipe.h"

Render_pipe::Render_pipe()
  :gWindow_(nullptr), gRenderer_(nullptr), gFont_(nullptr) {}

Render_pipe::~Render_pipe() {
  TTF_CloseFont(gFont_);
  SDL_DestroyRenderer(gRenderer_);
  SDL_DestroyWindow(gWindow_);
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

//Public
bool Render_pipe::init() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "SDL could not init! SDL error: "
      << SDL_GetError() << '\n';
    return false;
  }
  if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
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
  if (gWindow_ == nullptr) {
    std::cout << "Window could not be created! SDL Error: "
      << SDL_GetError() << '\n';
    return false; 
  }
  gRenderer_ = SDL_CreateRenderer(
    gWindow_, -1, SDL_RENDERER_ACCELERATED |
    SDL_RENDERER_PRESENTVSYNC
  );
  if (gRenderer_ == nullptr) {
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
  gFont_ = TTF_OpenFont(FILE_PATH_FONT, 35);
  if (nullptr == gFont_) {
    std::cout << "Failed to load font! SDL_ttf Error: "
      << TTF_GetError() << '\n';
    return false;
  }
  return true;
}

//Getters
SDL_Window* Render_pipe::get_window() const {
  return gWindow_;
}
  
SDL_Renderer* Render_pipe::get_renderer() const {
  return gRenderer_;
}

TTF_Font* Render_pipe::get_font() const {
  return gFont_;
}
