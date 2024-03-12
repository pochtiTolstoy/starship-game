#ifndef ENEMY_H_
#define ENEMY_H_

#include "../util/util.h"
#include "../util/constants.h"
#include "../texture/LTexture.h"
#include "planet.h"

using r_data = render_rotation_data;

class Enemy {
public:
  Enemy();
  ~Enemy();
  void render(Render_pipe&);
  void set_angle(double angle);
  void set_texture(const LTexture&);
  bool detect_planet_collision(const Planet&);
  void reinit();
  bool is_planet_hitted(const Planet&);
  bool move(double delta_time);

  //Getters
  bool is_alive() const;
  double get_angle() const;
  int get_x() const;
private:
  void calc_speed();
  const LTexture* pTexture_;
  int weight_;
  int height_;
  int x_pos_;
  int y_pos_;
  int shift_;
  r_data render_; 
  bool draw_;
  bool first_spawn_;
  static int enemy_on_map_;
};

#endif /* ENEMY_H_ */
