#include "enemy.h"

Enemy::Enemy(const LTexture& t, double angle): gTexture_(t) {
  //Linear data
  w_ = t.get_width();
  h_ = t.get_height();
  x_pos_ = SPAWN_ENEMY_X;
  y_pos_ = (SCREEN_HEIGHT - height_) / 2;
  shift_enemy_ = rand() % ENEMY_SPEED_LEVELS + 1;
  frame_rate_ = rand() % ENEMY_FRAMERATE_LEVELS + 1;
  current_frame_ = 0;

  //Rotation data
  render_.angle = angle; 
  render_.center = { SCREEN_WIDTH / 2 - x_pos_, height_ / 2 };
  render_.flip = SDL_FLIP_NONE;

  //Init conditions
  draw_ = false;
  first_spawn_ true;
}

Enemy::~Enemy() {}

void Enemy::reinit() {
  x_pos_ = SPAWN_ENEMY_X;
  y_pos_ = (SCREEN_HEIGHT - height_) / 2;
  current_frame_ = 0;
  render_.center = { SCREEN_WIDTH / 2 - x_pos_, height_ / 2 };
  draw_ = false;
  first_spawn_ = false;
}
