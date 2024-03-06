#ifndef LTEXTURE_H_
#define LTEXTURE_H_

#include "../util/util.h"

static const render_rotation_data RD = {
  .angle = 0.0,
  .center = {0, 0},
  .flip = SDL_FLIP_NONE
};

static const SDL_Color CL = { 255, 255, 255 };

class LTexture {
public:
  LTexture();
  ~LTexture();
  int get_width() const;
  int get_height() const;
  bool loadFromRenderedText(std::string text, SDL_Color);
  bool loadFromFile(std::string path, const SDL_Color& c = CL);
  void render(
    int x, 
    int y, 
    SDL_Rect* clip = nullptr, 
    const render_rotation_data& r = RD
  ) const;
  void setColor(Uint8 red, Uint8 green, Uint8 blue);
  void setBlendMode(SDL_BlendMode blending);
  void setAlpha(Uint8 alpha);
  void free();
private:
  SDL_Texture* texture_;
  int width_;
  int height_;
};

#endif /* LTEXTURE_H_ */
