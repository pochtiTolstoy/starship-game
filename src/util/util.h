// Header file with libraries and structures

#ifndef UTIL_H_
#define UTIL_H_

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

struct render_rotation_data {
  double angle;
  SDL_Point center;
  SDL_RendererFlip flip;
};

int eu_mod(int num, int mod);

#endif /* UTIL_H_ */
