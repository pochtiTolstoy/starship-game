#ifndef LEVEL_IMAGES_H_
#define LEVEL_IMAGES_H_

#include "../texture/LTexture.h"
#include "../util/constants.h"
#include "../util/util.h"

class Level_images {
public:
  Level_images() = delete;
  Level_images(Render_pipe &);
  ~Level_images();
  void render(Render_pipe &rp);
  int get_width(int i = 0) const;
  int get_height(int i = 0) const;
  int get_x() const;
  int get_y() const;
  int get_bottom_y(int i = 0) const;
  void set_position(int x, int y);

private:
  void init_images(Render_pipe &rp);

  LTexture level_images_arr_[NUM_LEVEL_IMAGES];
  int width_;
  int height_;
  int x_pos_;
  int y_pos_;
  int images_gap_;
};

#endif /* LEVEL_IMAGES_H_ */
