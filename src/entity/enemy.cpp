#include "enemy.h"

int Enemy::enemy_on_map_ = 0;

Enemy::Enemy(): pTexture_(nullptr) {
  //Linear data
  weight_ = 0;
  height_ = 0;
  x_pos_ = SPAWN_ENEMY_X;
  y_pos_ = (SCREEN_HEIGHT - height_) / 2;
  calc_speed();

  //Rotation data
  render_.angle = 0; 
  render_.center = { SCREEN_WIDTH / 2 - x_pos_, height_ / 2 };
  render_.flip = SDL_FLIP_NONE;

  //Init conditions
  draw_ = false;
  first_spawn_ = true;
}

void Enemy::set_angle(double angle) {
  render_.angle = angle;
}

void Enemy::set_texture(const LTexture& t) {
  pTexture_ = &t;
  weight_ = t.get_width();
  height_ = t.get_height();
  y_pos_ = (SCREEN_HEIGHT - height_) / 2;
  render_.center = { SCREEN_WIDTH / 2 - x_pos_, height_ / 2 };
}

Enemy::~Enemy() {}

void Enemy::reinit() {
  x_pos_ = SPAWN_ENEMY_X;
  y_pos_ = (SCREEN_HEIGHT - height_) / 2;
  calc_speed();
  render_.center = { SCREEN_WIDTH / 2 - x_pos_, height_ / 2 };
  draw_ = false;
  --enemy_on_map_;
  first_spawn_ = false;
}

void Enemy::render(Render_pipe& rp) {
  pTexture_->render(rp, x_pos_, y_pos_, nullptr, render_); 
}

bool Enemy::move(double delta_time) {
  int spawn_chance = first_spawn_ ? RAND_SPAWN_FIRST : RAND_SPAWN; 
  //std::cout << "SPAWN CHANCE: " << spawn_chance << '\n';
  //std::cout << "Enemies on map: " << enemy_on_map_ << '\n';
  //DELETE
  if (!draw_ && enemy_on_map_ < 12 && rand() % spawn_chance == 0) {
    draw_ = true;
    ++enemy_on_map_;
    return false;
  }
  if (draw_) {
    x_pos_ += std::floor(shift_ * delta_time);
    render_.center.x -= std::floor(shift_ * delta_time);
  }
  return true;
}

bool Enemy::detect_planet_collision(const Planet& pl) {
  if (!is_planet_hitted(pl)) return false;
  reinit();
  return true;
}

bool Enemy::is_planet_hitted(const Planet& pl) {
  return std::abs(SCREEN_WIDTH / 2 - x_pos_ - weight_ / 4)  <= PLANET_HITBOX &&
         std::abs(SCREEN_HEIGHT / 2 - y_pos_ - height_ / 4) <= PLANET_HITBOX; 
}

bool Enemy::is_alive() const { return draw_; }

double Enemy::get_angle() const { return render_.angle; }

int Enemy::get_x() const { return x_pos_; }

void Enemy::calc_speed() {
  if (rand() % 6 == 0) {
    shift_ = rand() % 40 + 250;
    //std::cout << "I AM VERY FAST!\n";
  } else {
    shift_ = 60;
    //shift_ = rand() % 90 + 70; 
  }
}
