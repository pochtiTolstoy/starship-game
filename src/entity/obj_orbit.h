#ifndef OBJ_ORBIT_H_
#define OBJ_ORBIT_H_

#include "../util/util.h"
#include "../texture/LTexture.h"
#include "ship.h"
#include "orbit.h"

using r_data = render_rotation_data;

class Obj_orbit {
  //friend class Orbit;
public:
  Obj_orbit() = delete;
  Obj_orbit(const LTexture&);
  ~Obj_orbit();
  void render(Render_pipe&);
  void calc_spawn(const Ship&, const Orbit&);
  bool detect_collision(const Ship&);
private:
  bool check_angle() const;

  const LTexture& gObjTexture_;
  int width_;
  int height_;
  int x_pos_;
  int y_pos_;
  r_data render_; 
  bool draw_;
};

#endif /* OBJ_ORBIT_H_ */
