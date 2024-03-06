#ifndef GAME_CONTROLS_H_
#define GAME_CONTROLS_H_

#include "../util/util.h"

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
