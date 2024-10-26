#ifndef MINE_H_
#define MINE_H_

#include "../texture/LTexture.h"
#include "../util/constants.h"
#include "../util/render_pipe.h"
#include "../util/util.h"
#include "enemy.h"

using r_data = render_rotation_data;

class Mine {
public:
  Mine();
  ~Mine();
  void set_texture(const LTexture &);
  void render(Render_pipe &) const;
  void drop(int y_pos, double angle);
  void death();
  bool is_alive() const;
  void detect_collision(Enemy *);

private:
  bool check_angle(double angle1, double angle2) const;
  int get_image_width() const;
  int get_image_height() const;

  // Linear data
  int width_;
  int height_;
  int x_pos_;
  int y_pos_;
  bool alive_;

  // Angular data
  r_data render_;

  // Image data
  const LTexture *pTexture_;
};

#endif /* MINE_H_ */
