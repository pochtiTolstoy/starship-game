#ifndef LTEXTURE_H_
#define LTEXTURE_H_

#include "../util/render_pipe.h"
#include "../util/util.h"

using r_data = render_rotation_data;

class LTexture {
public:
  LTexture();
  ~LTexture();
  int get_width() const;
  int get_height() const;
  bool loadFromRenderedText(Render_pipe &, const std::string &text, SDL_Color,
                            int ind = 0);
  bool loadFromRenderedLongText(Render_pipe &, const std::string &,
                                const SDL_Color &, int ind = 0);
  bool loadFromFile(Render_pipe &, std::string path,
                    const SDL_Color &color = CUT_COLOR);
  void render(Render_pipe &, int x, int y, SDL_Rect *clip = nullptr,
              const r_data &r = RD) const;
  void setColor(Uint8 red, Uint8 green, Uint8 blue);
  void setBlendMode(SDL_BlendMode blending);
  void setAlpha(Uint8 alpha);
  void free();

  // Class constants
  static constexpr SDL_Color CUT_COLOR = {0xFF, 0xFF, 0xFF};
  static constexpr r_data RD = {0.0, {0, 0}, SDL_FLIP_NONE};

private:
  SDL_Texture *texture_;
  int width_;
  int height_;
};

#endif /* LTEXTURE_H_ */
