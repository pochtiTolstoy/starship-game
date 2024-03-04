#ifndef GAME_CONTROLS_H_
#define GAME_CONTROLS_H_

#include <SDL2/SDL.h>

struct render_data {
  double angle;
  SDL_Point center;
  SDL_RendererFlip flip;
};

struct color {
  int R;
  int G;
  int B;
};

struct ship {
  int w;
  int h;
	int x_pos;
	int y_pos;
	int image;
	int shift_ship;
  render_data rd;
  int curr_lifes;
  int max_lifes;
  int max_bullets;
  int curr_bullets;
  int cooldown;
  int cooldown_timer;
  int kills;
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
  bool draw;
  bool first_spawn;
};

struct planet {
  int max_lifes;
  int curr_lifes;
};

struct obj_health {
  int w;
  int h;
  int x_pos;
  int y_pos;
  render_data rd;
  bool draw;
};


#endif /* GAME_CONTROLS_H_ */
