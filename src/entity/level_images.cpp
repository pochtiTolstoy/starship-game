#include "level_images.h"

Level_images::Level_images(Render_pipe &rp) : x_pos_(0), y_pos_(0) {
  init_images(rp);
  width_ = get_width();
  height_ = get_height();
  images_gap_ = 120;
}

void Level_images::render(Render_pipe &rp) {
  for (int i = 0; i < NUM_LEVEL_IMAGES; ++i) {
    level_images_arr_[i].render(rp, x_pos_,
                                y_pos_ + i * (images_gap_ + height_));
  }
}

void Level_images::init_images(Render_pipe &rp) {
  SDL_Color cut_color = {0xFF, 0x00, 0x00, 0xFF};
  bool success = false;
  for (int i = 0; i < NUM_LEVEL_IMAGES; ++i) {
    if (!level_images_arr_[i].loadFromFile(rp, FILE_PATHS_LEVEL_IMAGES[i],
                                           cut_color)) {
      std::cout << "Unable to load level images!\n";
      exit(EXIT_FAILURE);
    }
  }
}

Level_images::~Level_images() {
  for (int i = 0; i < NUM_LEVEL_IMAGES; ++i) {
    level_images_arr_[i].free();
  }
}

int Level_images::get_width(int i) const {
  return level_images_arr_[i].get_width();
}

int Level_images::get_height(int i) const {
  return level_images_arr_[i].get_height();
}

void Level_images::set_position(int x, int y) {
  x_pos_ = x;
  y_pos_ = y;
}

int Level_images::get_x() const { return x_pos_; }

int Level_images::get_y() const { return y_pos_; }

int Level_images::get_bottom_y(int i) const {
  return y_pos_ + get_height() + i * (images_gap_ + height_);
}
