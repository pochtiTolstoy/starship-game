#include "health_module.h"

Health_module::Health_module(Render_pipe &rp) {
  init_image(rp);
  width_ = get_image_width(STATES::FORWARD);
  height_ = get_image_height(STATES::FORWARD);
  speed_ = 130;
  reinit();
}

Health_module::~Health_module() {
  for (int i = 0; i < NUM_HEALTH_MODULE_TEXTURES; ++i) {
    gTextures_[i].free();
  }
}

bool Health_module::render(Render_pipe &rp, double delta_time,
                           Obj_health oh_arr[]) {
  if (!alive_ || ind_target_ == -1)
    return false;
  bool success = false;
  if (!get_heart_)
    move_to_heart(delta_time, oh_arr[ind_target_]);
  else
    success = move_to_planet(delta_time);
  render_image(rp);
  return success;
}

void Health_module::render_image(Render_pipe &rp) const {
  if (!alive_)
    return;
  gTextures_[image_].render(rp, x_pos_, y_pos_, nullptr, render_);
}

void Health_module::init_image(Render_pipe &rp) {
  for (int i = 0; i < NUM_HEALTH_MODULE_TEXTURES; ++i) {
    if (!gTextures_[i].loadFromFile(rp, FILE_PATH_HEALTH_MODULE[i])) {
      std::cout << "Failed to load health module texture!\n";
      exit(EXIT_FAILURE);
    }
  }
}

void Health_module::reinit() {
  image_ = STATES::FORWARD;
  x_pos_ = (SCREEN_WIDTH - width_) / 2;
  y_pos_ = (SCREEN_HEIGHT - height_) / 2;
  alive_ = false;
  get_heart_ = false;
  render_.angle = 0;
  render_.center = {width_ / 2, height_ / 2};
  render_.flip = SDL_FLIP_NONE;
  ind_target_ = -1;
}

int Health_module::get_image_width(int image) const {
  return gTextures_[image].get_width();
}

int Health_module::get_image_height(int image) const {
  return gTextures_[image].get_height();
}

void Health_module::calc_spawn(const Ship &sd, const Planet &pl,
                               Obj_health oh_arr[], int n) {
  if (alive_)
    return;
  bool lifes_full = pl.get_curr_lifes() >= pl.get_max_lifes() &&
                    sd.curr_lifes_ >= sd.max_lifes_;
  if (sd.kills_ < 150 || lifes_full) {
    alive_ = false;
    return;
  }
  bool alive_exist = false;
  for (int i = 0; i < n; ++i) {
    if (oh_arr[i].is_alive()) {
      alive_exist = true;
      render_.angle = oh_arr[i].get_angle();
      ind_target_ = i;
      break;
    }
  }
  if (!alive_exist) {
    alive_ = false;
    return;
  }
  alive_ = true;
}

bool Health_module::detect_collision_heart(Obj_health &oh) const {
  if (!alive_)
    return false;
  if (oh.get_angle() != render_.angle) {
    std::cout << "Something wrong with angles...\n";
    std::cout << "HEART ANGLE: " << oh.get_angle() << ", "
              << "MODULE ANGLE: " << render_.angle << '\n';
  }
  int mid_module = y_pos_ + height_ / 2;
  int mid_heart = oh.get_y_pos() + oh.get_image_height() / 2;
  if (std::abs(mid_module - mid_heart) <= 20) {
    return true;
  }
  return false;
}

bool Health_module::detect_collision_planet() const {
  if (!alive_)
    return false;
  int mid_module = y_pos_ + height_ / 2;
  if (std::abs(SCREEN_HEIGHT / 2 - mid_module) <= 50) {
    return true;
  }
  return false;
}

void Health_module::move_to_heart(double delta_time, Obj_health &oh) {
  // Don't change center for render data
  if (!oh.is_alive())
    reinit();
  if (detect_collision_heart(oh)) {
    oh.kill();         // Kill heart
    get_heart_ = true; // Means that now we will go to planet
    image_ = STATES::BACKWARD;
    return;
  }
  y_pos_ -= std::floor(speed_ * delta_time);
  render_.center.y += std::floor(speed_ * delta_time);
}

bool Health_module::move_to_planet(double delta_time) {
  if (detect_collision_planet()) {
    bool save_heart = get_heart_;
    reinit();
    return save_heart;
  }
  y_pos_ += std::floor(speed_ * delta_time);
  render_.center.y -= std::floor(speed_ * delta_time);
  return false;
}
