#ifndef UI_H_
#define UI_H_

#include "../util/constants.h"
#include "../util/render_pipe.h"
#include "../util/util.h"
#include "mine.h"
#include "planet.h"
#include "ship.h"

// Need for small textures, menu buttons, and ui in game process
class UI {
public:
  enum IMAGES {
    RED_HEART,
    BLACK_HEART,
    BULLET,
    EMPTY_BULLET,
    ORBIT_ELEMENT,
    BLUE_BULLET
  };
  enum ENEMY { METEOR };
  enum BACKGROUND { GAME_BACK1, MENU_BACK, GAME_BACK2 };

public:
  UI() = delete;
  UI(Render_pipe &);
  ~UI();
  void render_planet_health(Render_pipe &, const Planet &) const;
  void render_ship_health(Render_pipe &, const Ship &) const;
  void render_ship_bullets(Render_pipe &, const Ship &) const;
  void render_background(Render_pipe &) const;
  int get_image_width(int image) const;
  int get_image_height(int image) const;
  const LTexture &get_ui_texture(int image) const;
  const LTexture &get_enemy_texture(int image) const;
  const LTexture &get_mine_texture() const;
  void reset_image_background(BACKGROUND image);

private:
  int calc_render_x(int image, int obj_num, int objs_in_ui_bar) const;
  int calc_render_y(int i) const;
  BACKGROUND image_background_;

  LTexture gUITextures_[NUM_UI_TEXTURES];
  LTexture gEnemyTextures_[NUM_ENEMY_TEXTURES];
  LTexture gBackground_[NUM_BACKGROUNDS];
  LTexture gMineTexture_;
};

#endif /* UI_H_ */
