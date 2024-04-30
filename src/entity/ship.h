#ifndef SHIP_H_
#define SHIP_H_

#include "../util/util.h"
#include "../util/constants.h"
#include "../util/render_pipe.h"
#include "../util/timer.h"
#include "../texture/LTexture.h"
#include "enemy.h"

using r_data = render_rotation_data;

namespace STATES {
  enum { 
    DEFAULT, MOVE_FORWARD, SHOOT, MOVE_BACKWARD, RELOAD, TRIPLE
  };
}
namespace GUN_STATES {
  enum { DEFAULT, TRIPLE };
}

class Ship {
public:
  Ship() = delete;
  Ship(
    Render_pipe& rp, 
    int max_lifes = 2, int max_bullets = 6, int cooldown = 1500
  );
  ~Ship();
  void render(Render_pipe&);
  void process_shooting(Enemy*);
  void shoot(Enemy*);
  void detect_collision(Enemy*);
  void change_shoot_animation();
  void calc_cooldown();
  void move(double delta_time);
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
  bool is_angle_sync(double, const Enemy&);
  void triple_shoot(Enemy*);
  void default_shoot(Enemy*);

//CHANGE TO PRIVATE LATER
public:
  //Render linear data
  int width_;
  int height_;
  int x_pos_;
  int y_pos_;
  int vel_r_;
  int vel_ang_;
  bool moving_r_;
  bool moving_ang_;

  //Render rotation data
  r_data render_;

  //Stats
  int curr_lifes_;
  int max_lifes_;
  int max_bullets_;
  int curr_bullets_;
  int cooldown_;
  //int cooldown_timer_;
  LTimer cooldown_timer_;
  int kill_streak_;
  static const int kill_streak_triple_ = 8;
  int kills_;

  //Visuals
  int image_;
  int gun_state_;
  LTexture gShipTextures_[NUM_SHIP_TEXTURES];
};

#endif /* SHIP_H_ */
