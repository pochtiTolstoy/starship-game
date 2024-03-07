#ifndef OBJ_HEALTH_H_
#define OBJ_HEALTH_H_

#include "../texture/LTexture.h"

class Obj_health {
public:
  Obj_health();
  ~Obj_health();
private:
  //Takes texture from UI class
  const LTexture& gHealthTexture_;
};

#endif /* OBJ_HEALTH_H_ */
