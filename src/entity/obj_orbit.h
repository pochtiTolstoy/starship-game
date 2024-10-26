#ifndef OBJ_ORBIT_H_
#define OBJ_ORBIT_H_

#include "../texture/LTexture.h"
#include "../util/util.h"
#include "orbit.h"
#include "ship.h"

using r_data = render_rotation_data;
class Orbit;

class Obj_orbit {
public:
  Obj_orbit() = delete;
  Obj_orbit(const LTexture &);
  ~Obj_orbit();
  void render(Render_pipe &);
  void calc_spawn(const Ship &, const Orbit &);

  bool detect_collision(const Ship &);

  // Getters:
  int get_y() const;
  const r_data &get_r_data() const;

private:
  bool check_angle() const;
  bool check_kills(int kills) const;
  const LTexture &gObjTexture_;
  int width_;
  int height_;
  int x_pos_;
  int y_pos_;
  r_data render_;
  bool draw_;
};

#endif /* OBJ_ORBIT_H_ */
