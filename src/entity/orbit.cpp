#include "orbit.h"

Orbit::Orbit(Render_pipe& rp) {
  init_images(rp);
  image_ = STATES::DEFAULT;
  speed_ = 20;
  width_ = get_image_width(image_);
  height_ = get_image_height(image_);
  x_pos_ = (SCREEN_WIDTH - width_) / 2;
  y_pos_ = (SCREEN_HEIGHT - height_) / 2 - 500;
  render_.angle = 0;
  render_.center = { width_ / 2, height_ / 2 + 500 };
  render_.flip = SDL_FLIP_NONE;
  alive_ = false;
  curr_lifes_ = 0;
  max_lifes_ = 6;
}

Orbit::~Orbit() {
  std::cout << "Clear Orbit class textures:\n";
  for (int i = 0; i < NUM_ORBIT_TEXTURES; ++i) {
    gOrbitTextures_[i].free();
  }
  std::cout << '\n';
}

void Orbit::render(Render_pipe& rp) {
  if (!alive_) return;
  gOrbitTextures_[image_].render(
    rp,
    x_pos_, y_pos_, nullptr, render_
  );
}

void Orbit::reinit(int y_pos, const r_data& ang_data) {
  image_ = STATES::DEFAULT; 
  speed_ = 20;
  y_pos_ = y_pos;
  render_ = ang_data;
  alive_ = true;
  curr_lifes_ = max_lifes_;
}

void Orbit::move(double delta_time) {
  if (!alive_) return;
  render_.angle -= (speed_ * delta_time); 
  //std::cout << "Orbit angles: " << (20 * delta_time) << '\n';
}

void Orbit::init_images(Render_pipe& rp) {
  SDL_Color cut_color = { 0xFF, 0xFF, 0xFF, 0xFF };
  for (int i = 0; i < NUM_ORBIT_TEXTURES; ++i) {
    if (!gOrbitTextures_[i].loadFromFile(rp, FILE_PATHS_ORBIT[i], cut_color)) {
      std::cerr << "Failed to load orbit textures!\n";
      exit(EXIT_FAILURE);
    }
  }
}

void Orbit::detect_collision(Enemy* e_arr) {
  if (!alive_) return;
  int coords_sync = (SCREEN_WIDTH - SCREEN_HEIGHT) / 2;
  double angle_sync = render_.angle + COORDS_SYNC;

  //std::cout << "ANGLE BEFORE CORRECTION: " << angle_sync << '\n';
  if (angle_sync < 0) {
    angle_sync += (-1) * (static_cast<int>(angle_sync) / 360) * 360 + 360;
  }
  //std::cout << "ANGLE AFTER CORRECTION: " << angle_sync << '\n';

  for (int i = 0; i < NUM_ENEMY_ON_MAP; ++i) {
    if (!e_arr[i].is_alive()) continue;
    if (check_angle(angle_sync, e_arr[i])) {
      int diff = y_pos_ + (get_image_height(image_) / 2) - 
        (e_arr[i].get_x() + e_arr[i].get_weight() / 2) + coords_sync;
      std::cout << "DIFF: " << std::abs(diff) << '\n';
      if (std::abs(diff) <= 90) {
        e_arr[i].reinit();
        --curr_lifes_;
      }
    }
  }
  if (curr_lifes_ <= 0) alive_ = false;
}


void Orbit::change_speed(double velocity) { //ANGULAR SPEED
  if (!alive_) return;
  speed_ += velocity; 
  change_animation_move(velocity);
}

void Orbit::change_animation_move(double velocity) {
  image_ = (velocity < 0) ? STATES::DEFAULT : STATES::MOVE;
}

bool Orbit::check_angle(double angle, const Enemy& e) {
  //std::cout << "ENEMY EPSILON: [" << e.get_angle() - 10 
    //<< ", " << e.get_angle() + 10 << "]\n";
  if (angle <= e.get_angle() + 10 && angle >= e.get_angle() - 10) {
    return true;
  }
  return false;
}

int Orbit::get_image_height(int image) const {
  return gOrbitTextures_[image].get_height();
}

int Orbit::get_image_width(int image) const {
  return gOrbitTextures_[image].get_width();
}

bool Orbit::is_alive() const {
  return alive_;
}
