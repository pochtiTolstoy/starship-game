#include "LTexture.h"

LTexture::LTexture()
  : texture_(nullptr), width_(0), height_(0) {}

LTexture::~LTexture() { 
  free(); 
}

bool LTexture::loadFromFile(
  Render_pipe& rp,
  std::string path, 
  const SDL_Color& c
) {
  free();
  SDL_Texture* newTexture = nullptr;
  SDL_Surface* loadedSurface = IMG_Load(path.c_str());
  if (nullptr == loadedSurface) {
    std::cout << "Unable to load image " << path
      << "! SDL_image Error: " << IMG_GetError() << '\n';
    return false;
  }
  SDL_SetColorKey(loadedSurface, SDL_TRUE,
    SDL_MapRGB(loadedSurface->format, c.r, c.g, c.b)
  );
  //!!
  newTexture = SDL_CreateTextureFromSurface(
    rp.get_renderer(), loadedSurface
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

bool LTexture::loadFromRenderedText(
  Render_pipe& rp,
  const std::string& texture_text, 
  SDL_Color text_color
) {
  free();
  SDL_Surface* text_surface = TTF_RenderText_Solid(
    rp.get_font(), texture_text.c_str(), text_color
  );
  if (nullptr == text_surface) {
    std::cout << "Unable to render text surface! "
      "SDL_ttf Error: " << TTF_GetError() << '\n';
    return false;
  }
  //!!
  texture_ = SDL_CreateTextureFromSurface(
    rp.get_renderer(), text_surface
  );
  if (nullptr == texture_) {
    std::cout << "Unable to create texture from "
      "rendered text! SDL Error: " << SDL_GetError() << '\n';
    SDL_FreeSurface(text_surface);
    return false;
  }
  width_ = text_surface->w;
  height_ = text_surface->h;
  SDL_FreeSurface(text_surface);
  return texture_ != nullptr;
}

bool LTexture::loadFromRenderedLongText(
  Render_pipe& rp,
  const std::string& texture_text,
  const SDL_Color& text_color
) {
  free();
  SDL_Surface* text_surface = TTF_RenderUTF8_Blended_Wrapped(
    rp.get_font(1), texture_text.c_str(), text_color, 600
  );
  if (nullptr == text_surface) {
    std::cout << "Unable to render long text surface! "
      "SDL_ttf Error: " << TTF_GetError() << '\n';
    return false;
  }
  texture_ = SDL_CreateTextureFromSurface(
    rp.get_renderer(), text_surface
  );
  if (nullptr == texture_) {
    std::cout << "Unable to creatae texture from "
      "rendered text! SDL Error: " << SDL_GetError() << '\n';
    return false;
  }
  width_ = text_surface->w;
  height_ = text_surface->h;
  SDL_FreeSurface(text_surface);
  return texture_ != nullptr;
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

void LTexture::render (
  Render_pipe& rp,
  int x, int y, 
  SDL_Rect* clip,
  const r_data& rd
) const {
  //Set fullsize texture
  SDL_Rect renderQuad = { x, y, width_, height_ };
  //Set slice of texture
  if (nullptr != clip) {
    renderQuad.w = clip->w;
    renderQuad.h = clip->h;
  }
  //!!
  SDL_RenderCopyEx(
    rp.get_renderer(), texture_, 
    clip, &renderQuad, 
    rd.angle, &rd.center, rd.flip
  );
}

int LTexture::get_width() const { return width_; }

int LTexture::get_height() const { return height_; }
