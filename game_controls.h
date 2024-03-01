#ifndef GAME_CONTROLS_H_
#define GAME_CONTROLS_H_

#include <SDL2/SDL.h>

struct game_controls {
	int frame;
  double angle;
	double shift_texture;
	int width;
	int height;
  SDL_RendererFlip flip;
  SDL_Point* center;
};

#endif /* GAME_CONTROLS_H_ */
