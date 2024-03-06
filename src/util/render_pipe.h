#ifndef RENDER_PIPE_H_
#define RENDER_PIPE_H_

#include "util.h"
#include "../constants.h"

class Render_pipe {
public:
  Render_pipe();
  ~Render_pipe();
  bool init(); 
  bool loadMedia();
private:
  SDL_Window* gWindow_;
  SDL_Renderer* gRenderer_;
  TTF_Font* gFont_;

  //Textures without classes
  LTexture gEnemyTextures_[NUM_ENEMY_TEXTURES];
  LTexture gUITextures_[NUM_UI_TEXTURES];
  LTexture gBackground_;
  LTexture gTextTexture_;
};

#endif /* RENDER_PIPE_H_ */
