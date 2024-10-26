#ifndef RENDER_PIPE_H_
#define RENDER_PIPE_H_

#include "constants.h"
#include "util.h"

class Render_pipe {
public:
  Render_pipe();
  ~Render_pipe();
  bool init();

  // Getters
  SDL_Window *get_window() const;
  SDL_Renderer *get_renderer() const;
  TTF_Font *get_font(int ind = 0) const;

private:
  SDL_Window *gWindow_;
  SDL_Renderer *gRenderer_;
  // TTF_Font* gFont_;
  // TTF_Font* gFont_small_;
  TTF_Font *gFonts_[NUM_FONTS];
};

#endif /* RENDER_PIPE_H_ */
