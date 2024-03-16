#include "ui.h"

UI::UI(Render_pipe& rp) {
  for (int i = 0; i < NUM_UI_TEXTURES; ++i) {
    if (!gUITextures_[i].loadFromFile(rp, FILE_PATHS_UI[i])) {
      std::cout << "Failed to load UI textures!\n";
      exit(EXIT_FAILURE);
    }
  }
  for (int i = 0; i < NUM_ENEMY_TEXTURES; ++i) {
    if (!gEnemyTextures_[i].loadFromFile(rp, FILE_PATHS_ENEMY[i])) {
      std::cout << "Failed to load enemy textures!\n";
      exit(EXIT_FAILURE);
    }
  }
  if (!gBackground_.loadFromFile(rp, FILE_PATH_BACKGROUND)) {
    std::cout << "Failed to load planet!\n";
    exit(EXIT_FAILURE);
  }
}

UI::~UI() {
  for (int i = 0; i < NUM_UI_TEXTURES; ++i) {
    gUITextures_[i].free();
  }
  for (int i = 0; i < NUM_ENEMY_TEXTURES; ++i) {
    gEnemyTextures_[i].free();
  }
  gBackground_.free();
}

const LTexture& UI::get_ui_texture(int image) const {
  return gUITextures_[image];
}

const LTexture& UI::get_enemy_texture(int image) const {
  return gEnemyTextures_[image];
}

void UI::render_planet_health(Render_pipe& rp, const Planet& pl) const {
  //DRAW RED HEARTS
  for (int i = 0; i < pl.get_curr_lifes(); ++i) {
    gUITextures_[IMAGES::RED_HEART].render(
      rp, 
      calc_render_x(IMAGES::RED_HEART, i, pl.get_max_lifes()),
      calc_render_y(IMAGES::RED_HEART) + SHIFT_HEART_PLANET_Y
    );
  }
  //DRAW BLACK HEARTS
  for (int i = pl.get_curr_lifes(); i < pl.get_max_lifes(); ++i) {
    gUITextures_[IMAGES::BLACK_HEART].render(
      rp,
      calc_render_x(IMAGES::BLACK_HEART, i, pl.get_max_lifes()),
      calc_render_y(IMAGES::BLACK_HEART) + SHIFT_HEART_PLANET_Y
    );
  }
}

void UI::render_ship_health(Render_pipe& rp, const Ship& sd) const {
  //DRAW RED HEARTS
  for (int i = 0; i < sd.curr_lifes_; ++i) {
    gUITextures_[IMAGES::RED_HEART].render(
      rp,
      calc_render_x(IMAGES::RED_HEART, i, sd.max_lifes_),
      calc_render_y(IMAGES::RED_HEART) +
      SHIFT_HEART_PLANET_Y + SHIFT_HEART_SHIP_Y
    );
  }
  //DRAW BLACK HEARTS
  for (int i = sd.curr_lifes_; i < sd.max_lifes_; ++i) {
    gUITextures_[IMAGES::BLACK_HEART].render(
      rp,
      calc_render_x(IMAGES::BLACK_HEART, i, sd.max_lifes_),
      calc_render_y(IMAGES::BLACK_HEART) +
      SHIFT_HEART_PLANET_Y + SHIFT_HEART_SHIP_Y
    );
  }
}

void UI::render_ship_bullets(Render_pipe& rp, const Ship& sd) const {
  int bullet_image = 0;
  if (sd.gun_state_ == GUN_STATES::DEFAULT) {
    bullet_image = IMAGES::BULLET;
  } else {
    bullet_image = IMAGES::BLUE_BULLET;
  }
  for (int i = 0; i < sd.curr_bullets_; ++i) {
    gUITextures_[bullet_image].render(
      rp,
      calc_render_x(bullet_image, i, sd.max_bullets_),
      calc_render_y(bullet_image) +
      SHIFT_HEART_PLANET_Y + 2 * SHIFT_HEART_SHIP_Y
    );
  }
  for (int i = sd.curr_bullets_; i < sd.max_bullets_; ++i) {
    gUITextures_[IMAGES::EMPTY_BULLET].render(
      rp,
      calc_render_x(IMAGES::EMPTY_BULLET, i, sd.max_bullets_),
      calc_render_y(IMAGES::EMPTY_BULLET) +
      SHIFT_HEART_PLANET_Y + 2 * SHIFT_HEART_SHIP_Y
    );
  }
}

void UI::render_background(Render_pipe& rp) const {
  gBackground_.render(rp, 0, 0);
}

//======================Helper methods===========================
int UI::get_image_width(int image) const {
  return gUITextures_[image].get_width();
}

int UI::get_image_height(int image) const {
  return gUITextures_[image].get_height();
}

int UI::calc_render_x(int image, int obj_num, int objs_in_ui_bar) const {
  //Object order in centralized ui bar
  int order_in_ui_bar = obj_num - objs_in_ui_bar / 2;
  //x position in ui bar
  int x_pos = order_in_ui_bar * get_image_width(image);
  //Return object x position centralized
  return SCREEN_WIDTH / 2 + x_pos;
}

int UI::calc_render_y(int image) const {
  return (SCREEN_HEIGHT - get_image_height(image)) / 2;
}
