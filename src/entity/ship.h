#ifndef SHIP_H_
#define SHIP_H_

#include "../util/util.h"
#include "../util/constants.h"
#include "../util/render_pipe.h"
#include "../texture/LTexture.h"
#include "enemy.h"

using r_data = render_rotation_data;

class Ship {
public:
  Ship() = delete;
  Ship(
    Render_pipe& rp, 
    int max_lifes = 2, int max_bullets = 6, int cooldown = 100
  );
  ~Ship();
  void render(Render_pipe&);
  void shoot(Enemy*);
  void detect_collision(Enemy*);
  void change_shoot_animation();
  void calc_cooldown();
  bool is_fighting() const;

private:
  //Helper methods
  void render_image(Render_pipe&) const;
  void render_high_image(Render_pipe&) const;
  void init_images(Render_pipe&);
  const SDL_Color& get_cut_color(int image) const;
  r_data calc_rotation_high() const;
  int calc_high_y() const;
  int get_image_height(int image) const;
  int get_image_width(int image) const;
  bool is_reloaded() const;
  bool is_image_high() const;
  bool is_angle_sync(const Enemy&);

//CHANGE TO PRIVATE LATER
public:
  //Render linear data
  int width_;
  int height_;
  int x_pos_;
  int y_pos_;

  //Render rotation data
  r_data render_;

  //Stats
  int curr_lifes_;
  int max_lifes_;
  int max_bullets_;
  int curr_bullets_;
  int cooldown_;
  int cooldown_timer_;
  int kills_;

  //Visuals
  int image_;
  LTexture gShipTextures_[NUM_SHIP_TEXTURES];

public:
  enum STATES { 
    DEFAULT, MOVE_FORWARD, SHOOT, MOVE_BACKWARD, RELOAD 
  };
};

#endif /* SHIP_H_ */
