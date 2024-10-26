// Header file with libraries and structures

#ifndef UTIL_H_
#define UTIL_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

struct render_rotation_data {
  double angle;
  SDL_Point center;
  SDL_RendererFlip flip;
};

int eu_mod(int num, int mod);

#endif /* UTIL_H_ */
