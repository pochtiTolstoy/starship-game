#ifndef HEALTH_MODULE_H_
#define HEALTH_MODULE_H_

#include "../util/util.h"
#include "../util/render_pipe.h"
#include "../util/constants.h"
#include "../texture/LTexture.h"
#include "planet.h"
#include "ship.h"
#include "obj_health.h"

using r_data = render_rotation_data;

class Health_module {
public:
  Health_module() = delete;
  Health_module(Render_pipe&);
  ~Health_module();
  bool render(Render_pipe&, double delta_time, Obj_health oh_arr[]);
  void calc_spawn(
    const Ship&, 
    const Planet&, 
    Obj_health oh_arr[],
    int n
  );
private:
  void render_image(Render_pipe&) const;
  void init_image(Render_pipe&);
  void reinit();
  bool detect_collision_heart(Obj_health& oh) const;
  bool detect_collision_planet() const;
  int get_image_width(int image) const;
  int get_image_height(int image) const;
  void move_to_heart(double delta_time, Obj_health& oh);
  bool move_to_planet(double delta_time);

  //Linear data:
  int width_;
  int height_;
  int y_pos_;
  int x_pos_;
  bool alive_;
  bool get_heart_;
  int ind_target_;
  int speed_;

  //Angular data:
  r_data render_;

  //Image data:
  LTexture gTextures_[NUM_HEALTH_MODULE_TEXTURES];
  int image_;

  enum STATES {
    FORWARD, BACKWARD 
  };
};

#endif /* HEALTH_MODULE_H_ */
