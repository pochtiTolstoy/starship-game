#ifndef HEALTH_MODULE_H_
#define HEALTH_MODULE_H_

#include "../util/util.h"
#include "../util/render_pipe.h"
#include "../util/constants.h"
#include "../texture/LTexture.h"
#include "planet.h"
#include "obj_health.h"

using r_data = render_rotation_data;

class Health_module {
public:
  Health_module() = delete;
  Health_module(Render_pipe&);
  ~Health_module();
private:
  void init_image(Render_pipe&);
  int get_image_width() const;
  int get_image_height() const;

  //Linear data:
  int width_;
  int height_;
  int y_pos_;
  int x_pos_;
  bool alive_;
  bool get_heart_;

  //Angular data:
  r_data render_;

  //Image data:
  LTexture gTexture_;
};

#endif /* HEALTH_MODULE_H_ */
