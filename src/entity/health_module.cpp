#include "health_module.h"

Health_module::Health_module(Render_pipe& rp) {
  init_image(rp); 
  width_ = get_image_width();
  height_ = get_image_height();
  x_pos_ = (SCREEN_WIDTH - width_) / 2;
  y_pox_ = (SCREEN_HEIGHT - height_) / 2;
  alive_ = false;
  get_heart_ = false;
  render_.angle = 0;
  render_.center = { width_ / 2, height_ / 2 };
  render_.flie = SDL_FLIP_NONE;
}

Health_module::~Health_module() {
  gTexture_.free();
}

void Health_module::render(Render_pipe& rp) {
  if (!alive_) return;
  if (!get_heart_) move_to_heart();
  else move_to_planet();
}

void Health_module::init_image(Render_pipe& rp) {
  if (!gTexture_.loadFromFile(rp, FILE_PATH_HEALTH_MODULE)) {
    std::cout << "Failed to load health module texture~\n";
    exit(EXIT_FAILURE);
  }
}

int Health_module::get_image_width() const {
  return gTexture_.get_width();
}

int Health_module::get_image_height() const {
  return gTexture_.get_height();
}

void Health_module::calc_spawn(int kills, const Planet& pl) {
  alive_ = (kills >= 60 && pl.get_curr_lifes != pl.get_max_lifes);
}

void Health_moude::detect_collision() {
  if (!alive_) return;
}
