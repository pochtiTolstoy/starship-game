#ifndef UI_H_
#define UI_H_

#include "../util/util.h"
#include "../util/constants.h"
#include "../util/render_pipe.h"
#include "planet.h"
#include "ship.h"

class UI {
public:
  UI() = delete;
  UI(Render_pipe&);
  ~UI();
  void render_planet_health(Render_pipe&, const Planet&);
  void render_ship_health(Render_pipe&, const Ship&);
  void render_ship_bullets(Render_pipe&, const Ship&);
  void render_background(Render_pipe&);
  int get_image_width(int image) const;
  int get_image_height(int image) const;
  const LTexture& get_ui_texture(int image) const;
  const LTexture& get_enemy_texture(int image) const;
private:
  int calc_render_x(int image, int obj_num, int objs_in_ui_bar) const;
  int calc_render_y(int i) const;

  LTexture gUITextures_[NUM_UI_TEXTURES];
  LTexture gEnemyTextures_[NUM_ENEMY_TEXTURES];
  LTexture gBackground_;

public:
  enum IMAGES { RED_HEART, BLACK_HEART, BULLET, EMPTY_BULLET, ORBIT_ELEMENT };
  enum ENEMY { METEOR };
};

#endif /* UI_H_ */
