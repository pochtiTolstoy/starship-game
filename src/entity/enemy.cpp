#include "enemy.h"

int Enemy::enemy_on_map_ = 0;

Enemy::Enemy()
  : pTexture_(nullptr),
    width_(0),
    height_(0),
    x_pos_(0),
    y_pos_(0),
    render_({
      .angle = 0,
      .center = {0, 0},
      .flip = SDL_FLIP_NONE
    }),
    draw_(false),
    first_spawn_(true)
{
  //Linear data
  //weight_ = 0;
  //height_ = 0;
  //x_pos_ = SPAWN_ENEMY_X;
  //y_pos_ = (SCREEN_HEIGHT - height_) / 2;
  //x_pos_ = 0;
  //y_pos_ = 0;
  calc_speed();

  //Rotation data
  //render_.angle = 0; 
  //render_.center = { 0, 0 };
  //render_.flip = SDL_FLIP_NONE;

  //Init conditions
  //draw_ = false;
  //first_spawn_ = true;
}

void Enemy::set_angle(double angle) {
  render_.angle = angle;
}

void Enemy::set_texture(const LTexture& t) {
  pTexture_ = &t; //Pointer
  if (pTexture_ == nullptr) {
    std::cout << "ERROR: no texture loaded for enemy object!\n";
    exit(EXIT_FAILURE);
  }
  width_ = t.get_width();
  height_ = t.get_height();
  x_pos_ = (SCREEN_WIDTH - width_) / 2;
  y_pos_ = SPAWN_ENEMY_X - (SCREEN_WIDTH - SCREEN_HEIGHT) / 2;
  render_.center = { width_ / 2, SCREEN_HEIGHT / 2 - y_pos_ };
}

Enemy::~Enemy() {}

void Enemy::reinit() {
  //x_pos_ = SPAWN_ENEMY_X;
  //y_pos_ = (SCREEN_HEIGHT - height_) / 2;
  x_pos_ = (SCREEN_WIDTH - width_) / 2;
  y_pos_ = SPAWN_ENEMY_X - (SCREEN_WIDTH - SCREEN_HEIGHT) / 2;

  calc_speed();
  render_.center = { width_ / 2, SCREEN_HEIGHT / 2 - y_pos_ };
  draw_ = false;
  first_spawn_ = false;

  enemy_on_map_ = (enemy_on_map_ > 0) ? --enemy_on_map_ : 0;
}

void Enemy::render(Render_pipe& rp) {
  if (pTexture_ == nullptr) {
    std::cout << "ERROR: no texture loaded for enemy object!\n";
    exit(EXIT_FAILURE);
  }
  pTexture_->render(rp, x_pos_, y_pos_, nullptr, render_); 
}

bool Enemy::move(double delta_time) {
  int spawn_chance = first_spawn_ ? RAND_SPAWN_FIRST : RAND_SPAWN; 
  if (!draw_ && enemy_on_map_ < 12 && rand() % spawn_chance == 0) {
    draw_ = true;
    ++enemy_on_map_;
    return false;
  }
  if (draw_) {
    //x_pos_ += std::floor(shift_ * delta_time);
    //render_.center.x -= std::floor(shift_ * delta_time);
    y_pos_ += std::floor(shift_ * delta_time);
    render_.center.y -= std::floor(shift_ * delta_time);
  }
  return true;
}

bool Enemy::detect_planet_collision(const Planet& pl) {
  if (!is_planet_hitted(pl)) return false;
  reinit();
  return true;
}

bool Enemy::is_planet_hitted(const Planet& pl) {
  return std::abs(SCREEN_HEIGHT / 2 - y_pos_ - height_ / 2) <= 190;
}

bool Enemy::is_alive() const { return draw_; }

double Enemy::get_angle() const { return render_.angle; }

int Enemy::get_x() const { return x_pos_; }

int Enemy::get_y() const { return y_pos_; }

void Enemy::calc_speed() {
  if (rand() % 6 == 0) {
    shift_ = rand() % 40 + 250;
    //std::cout << "I AM VERY FAST!\n";
  } else {
    shift_ = rand() % 90 + 70; 
  }
}

int Enemy::get_width() const {
  return width_;
}

int Enemy::get_height() const {
  return height_;
}
