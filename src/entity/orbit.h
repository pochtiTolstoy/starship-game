#ifndef ORBIT_H_
#define ORBIT_H_

#include "../texture/LTexture.h"
#include "../util/render_pipe.h"
#include "../util/constants.h"
#include "enemy.h"

using r_data = render_rotation_data;

class Orbit {
public:
  Orbit() = delete;
  Orbit(Render_pipe&);
  ~Orbit();
  void render(Render_pipe&);
  void move(double delta_time);
  void detect_collision(Enemy*);
private:
  void init_images(Render_pipe&);
  int get_image_height(int image) const;
  int get_image_width(int image) const;
  bool check_angle(double angle, const Enemy&);
  
  //Linear data
  int width_;
  int height_;
  int x_pos_;
  int y_pos_;

  //Angular data
  r_data render_;

  //Image data
  LTexture gOrbitTextures_[NUM_ORBIT_TEXTURES];
  int image_;

  //Other
  enum STATES { DEFAULT };
};

#endif /* ORBIT_H_ */
