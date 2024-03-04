#include "LTexture.h"

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;

LTexture::LTexture()
  : texture_(nullptr), width_(0), height_(0) {}

LTexture::~LTexture() { free(); }

bool LTexture::loadFromFile(std::string path, const color& c) {
  free();
  SDL_Texture* newTexture = nullptr;
  SDL_Surface* loadedSurface = IMG_Load(path.c_str());
  if (nullptr == loadedSurface) {
    std::cout << "Unable to load image " << path
      << "! SDL_image Error: " << IMG_GetError() << '\n';
    return false;
  }
  SDL_SetColorKey(loadedSurface, SDL_TRUE,
    SDL_MapRGB(loadedSurface->format, c.R, c.G, c.B)
  );
  newTexture = SDL_CreateTextureFromSurface(
    gRenderer, loadedSurface
  );
  if (nullptr == newTexture) {
    std::cout << "Unable to create texture from " << path
      << "! SDL_Error: " << SDL_GetError() << '\n';
    SDL_FreeSurface(loadedSurface);
    return false;
  }
  width_ = loadedSurface->w;
  height_ = loadedSurface->h;
  SDL_FreeSurface(loadedSurface);
  texture_ = newTexture;
  return true;
}

void LTexture::free() {
  if (nullptr != texture_) {
    SDL_DestroyTexture(texture_);
    texture_ = nullptr;
    width_ = height_ = 0;
  }
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
  SDL_SetTextureColorMod(texture_, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending) {
  SDL_SetTextureBlendMode(texture_, blending);
}

void LTexture::setAlpha(Uint8 alpha) {
  SDL_SetTextureAlphaMod(texture_, alpha);
}

void LTexture::render(
  int x, int y, SDL_Rect* clip,
  const render_data& rd
) {
  //Set fullsize texture
  SDL_Rect renderQuad = { x, y, width_, height_ };
  //Set slice of texture
  if (nullptr != clip) {
    renderQuad.w = clip->w;
    renderQuad.h = clip->h;
  }
  SDL_RenderCopyEx(
    gRenderer, texture_, 
    clip, &renderQuad, 
    rd.angle, 
    &rd.center, 
    rd.flip
  );
}

int LTexture::getWidth() { return width_; }

int LTexture::getHeight() { return height_; }
