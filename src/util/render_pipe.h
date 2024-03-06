#ifndef RENDER_PIPE_H_
#define RENDER_PIPE_H_

#include "util.h"
#include "constants.h"

class Render_pipe {
public:
  Render_pipe();
  ~Render_pipe();
  bool init();

  //Getters
  SDL_Window* get_window() const;
  SDL_Renderer* get_renderer() const;
  TTF_Font* get_font() const;
private:
  SDL_Window* gWindow_;
  SDL_Renderer* gRenderer_;
  TTF_Font* gFont_;
};

#endif /* RENDER_PIPE_H_ */
