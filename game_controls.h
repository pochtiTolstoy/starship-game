#ifndef GAME_CONTROLS_H_
#define GAME_CONTROLS_H_

#include <SDL2/SDL.h>

struct render_data {
  double angle;
  SDL_Point center;
  SDL_RendererFlip flip;
};

struct ship {
  int w;
  int h;
	int x_pos;
	int y_pos;
	int image;
	int shift_ship;
  render_data rd;
};

struct enemy {
  int w;
  int h;
  int x_pos;
  int y_pos;
  int shift_enemy;
  int frame_rate;
  int current_frame;
  render_data rd; 
};


#endif /* GAME_CONTROLS_H_ */
