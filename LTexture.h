#ifndef LTEXTURE_H_
#define LTEXTURE_H_

#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "game_controls.h"

static const render_data rd = {
  .angle = 0.0,
  .center = {0, 0},
  .flip = SDL_FLIP_NONE
};

static const color cl = {
  .R = 255,
  .G = 255,
  .B = 255
};

class LTexture {
public:
  LTexture();
  ~LTexture();
  bool loadFromFile(std::string path, const color& c = cl);
  void free();
  void setColor(Uint8 red, Uint8 green, Uint8 blue);
  void setBlendMode(SDL_BlendMode blending);
  void setAlpha(Uint8 alpha);
  void render(
    int x, int y, SDL_Rect* clip = nullptr,
    const render_data& r = rd
  );
  int getWidth();
  int getHeight();
private:
  SDL_Texture* texture_;
  int width_;
  int height_;
};

#endif /* LTEXTURE_H_ */
