#ifndef ORBIT_H_
#define ORBIT_H_

#include "../texture/LTexture.h"
#include "../util/constants.h"
#include "../util/render_pipe.h"
#include "../util/util.h"
#include "enemy.h"
#include "mine.h"

using r_data = render_rotation_data;

class Orbit {
public:
  Orbit() = delete;
  Orbit(Render_pipe &);
  ~Orbit();
  void render(Render_pipe &);
  void render_mines(Render_pipe &);
  void move(double delta_time);
  void detect_collision(Enemy *);
  void change_speed(double velocity); // ANGULAR SPEED
  void change_animation_move(double velocity);
  bool is_alive() const;
  void reinit(int, const r_data &);
  void set_mines_texture(const LTexture &);
  void calc_drop_mine();
  void death();
  void process_mines_collision(Enemy *);

private:
  // Constants
  static const int NUM_MINES = 8;

  void init_images(Render_pipe &);
  int get_image_height(int image) const;
  int get_image_width(int image) const;
  bool check_angle(double angle1, double angle2);
  bool ready_to_drop_mine() const;

  // Linear data
  int width_;
  int height_;
  int x_pos_;
  int y_pos_;
  bool alive_;
  int curr_lifes_;
  int max_lifes_;
  Mine mine_arr_[NUM_MINES];
  int curr_mine_;
  int mine_throwed_; // Due to lifecycle

  // Angular data
  r_data render_;
  double speed_;

  // Image data
  LTexture gOrbitTextures_[NUM_ORBIT_TEXTURES];
  int image_;

  // Other
  enum STATES { DEFAULT, MOVE };
};

#endif /* ORBIT_H_ */
