#ifndef SHIP_H_
#define SHIP_H_

#include "../util.h"
#include "../constants.h"
#include "../texture/LTexture.h"

using render_rotation_data r_data;

class Ship {
public:
  Ship();
  ~Ship();
  void render() const;
private:
  //Helper methods
  void render_image() const;
  void render_high_image() const;
  const SDL_Color& get_cut_color(int image) const;
  bool is_reloaded() const;
  int calc_high_y() const;
  const r_data& calc_rotation_high() const;
  int get_height(int image = image_) const;
  int get_width(int image = image_) const;

  //Render linear data
  int width_;
  int height_;
  int x_pos_;
  int y_pos_;
  int shift_ship_;

  //Render rotation data
  r_data render_;

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
