#ifndef ORBIT_H_
#define ORBIT_H_

#include "../util/util.h"
#include "../texture/LTexture.h"
#include "../util/render_pipe.h"
#include "../util/constants.h"
#include "enemy.h"
#include "mine.h"

using r_data = render_rotation_data;

class Orbit {
public:
  Orbit() = delete;
  Orbit(Render_pipe&);
  ~Orbit();
  void render(Render_pipe&);
  void render_mines(Render_pipe&);
  void move(double delta_time);
  void detect_collision(Enemy*);
  void change_speed(double velocity); //ANGULAR SPEED
  void change_animation_move(double velocity);
  bool is_alive() const;
  void reinit(int, const r_data&);
  void set_mines_texture(const LTexture&);
  void calc_drop_mine();
private:
  //Constants
  static const int NUM_MINES = 12; 

  void init_images(Render_pipe&);
  int get_image_height(int image) const;
  int get_image_width(int image) const;
  bool check_angle(double angle1, double angle2);
  
  //Linear data
  int width_;
  int height_;
  int x_pos_;
  int y_pos_;
  bool alive_;
  int curr_lifes_;
  int max_lifes_;
  Mine mine_arr_[NUM_MINES];
  int curr_mine_;

  //Angular data
  r_data render_;
  double speed_;

  //Image data
  LTexture gOrbitTextures_[NUM_ORBIT_TEXTURES];
  int image_;

  //Other
  enum STATES { DEFAULT, MOVE };
};

#endif /* ORBIT_H_ */
