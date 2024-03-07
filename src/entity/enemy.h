#ifndef ENEMY_H_
#define ENEMY_H_

#include "../util/util.h"
#include "../util/constants.h"

using r_data = render_rotation_data;

class Enemy {
public:
  Enemy() = delete;
  Enemy(const LTexture&);
  ~Enemy();
private:
  const LTexture& gTexture_;
  int weight_;
  int height_;
  int x_pos_;
  int y_pos_;
  int shift_enemy_;
  int frame_rate_;
  int current_frame_;
  r_data render_; 
  bool draw_;
  bool first_spawn_;
};

#endif /* ENEMY_H_ */
