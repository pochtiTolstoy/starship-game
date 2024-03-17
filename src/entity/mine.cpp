#include "mine.h"

Mine::Mine()
  : width_(0),
    height_(0),
    x_pos_(0),
    y_pos_(0),
    alive_(false),
    render_({
      .angle = 0,
      .center = {0, 0},
      .flip = SDL_FLIP_NONE
    }),
    pTexture_(nullptr)
{}

Mine::~Mine() {}

void Mine::render(Render_pipe& rp) const {
  if (!alive_) return;
  if (pTexture_ == nullptr) {
    std::cout << "ERROR: no texture loaded for mine object!\n";
    exit(EXIT_FAILURE);
  }
  pTexture_->render(
    rp,
    x_pos_, y_pos_, nullptr, render_
  );
}

void Mine::set_texture(const LTexture& t) {
  pTexture_ = &t; //Pointer
  if (pTexture_ == nullptr) {
    std::cout << "ERROR: no texture loaded for enemy object!\n";
    exit(EXIT_FAILURE);
  }
  width_ = t.get_width();
  height_ = t.get_height();
}

void Mine::drop(int y_pos, double angle) {
  if (alive_) return;
  alive_ = true; 
  x_pos_ = (SCREEN_WIDTH - width_) / 2;
  y_pos_ = y_pos;
  render_.angle = angle;
  render_.center = { width_ / 2, SCREEN_HEIGHT / 2 - y_pos_ };
}

void Mine::death() {
  alive_ = false;
}

/*
void Mine::detect_collision(Enemy& e) {

}
*/

int Mine::get_image_width() const {
  return width_;
}

int Mine::get_image_height() const {
  return height_;
}

bool Mine::is_alive() const {
  return alive_;
}
