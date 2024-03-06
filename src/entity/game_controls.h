#ifndef GAME_CONTROLS_H_
#define GAME_CONTROLS_H_

#include "../util.h"

struct ship {
  int w;
  int h;
	int x_pos;
	int y_pos;
	int image;
  int curr_lifes;
  int max_lifes;
  render_rotation_data rd;
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
  render_rotation_data rd; 
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
  render_rotation_data rd;
  bool draw;
};

struct ui_killbar {
	int max_kills;
	int curr_kills;
	std::string text;
	SDL_Color color;
};

#endif /* GAME_CONTROLS_H_ */
