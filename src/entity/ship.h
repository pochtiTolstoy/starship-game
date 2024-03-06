#ifndef SHIP_H_
#define SHIP_H_

#include "../util.h"
#include "../constants.h"
#include "../texture/LTexture.h"

class Ship {
public:
  Ship();
  ~Ship();
private:
  //Helper methods
  const SDL_Color& get_cut_color(int image);

  //Render linear data
  int width_;
  int height_;
  int x_pos_;
  int y_pos_;
  int shift_ship_;

  //Render rotation data
  render_rotation_data render_;

  //Stats
  int curr_lifes_;
  int max_lifes_;
  int max_bullets_;
  int curr_nullets_;
  int cooldown_;
  int cooldown_timer_;
  int kills_;

  //Visuals
  int image_;
  LTexture gShipTextures_[NUM_SHIP_TEXTURES];

public:
  enum SHIP_STATES { 
    DEFAULT, MOVE_FORWARD, SHOOT, MOVE_BACKWARD, RELOAD 
  };
};

#endif /* SHIP_H_ */
