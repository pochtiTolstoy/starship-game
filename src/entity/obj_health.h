#ifndef OBJ_HEALTH_H_
#define OBJ_HEALTH_H_

#include "../util/util.h"
#include "../texture/LTexture.h"
#include "ship.h"

using r_data = render_rotation_data;

class Obj_health {
public:
  Obj_health() = delete;
  Obj_health(const LTexture&);
  ~Obj_health();
  void render(Render_pipe&);
  void calc_spawn();
  bool detect_collision(const Ship&);
  bool is_alive() const;
  const r_data& get_render_data() const;
  double get_angle() const;
  void kill();
  int get_y_pos() const;
  int get_image_height() const;

private:
  bool check_angle() const;

  //Takes texture from UI class
  const LTexture& gHealthTexture_;
  int width_;
  int height_;
  int x_pos_;
  int y_pos_;
  r_data render_; 
  bool draw_;
};

#endif /* OBJ_HEALTH_H_ */
