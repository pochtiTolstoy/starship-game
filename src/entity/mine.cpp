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

void Mine::detect_collision(Enemy* e) {
  if (!alive_) return;
  int mid_mine_y = y_pos_ + height_ / 2; 
  int mid_enemy_y = 0;
  double fix_angle = render_.angle;
  if (fix_angle < 0) {
    fix_angle += (-1) * (static_cast<int>(fix_angle) / 360) * 360 + 360;
  }

  for (int i = 0; i < NUM_ENEMY_ON_MAP; ++i) {
    if (!e[i].is_alive()) continue;
    mid_enemy_y = e[i].get_y() + e[i].get_height() / 2;
    if (check_angle(fix_angle, e[i].get_angle())) {
      if (std::abs(mid_mine_y - mid_enemy_y) <= 50) {
        death();
        e[i].reinit();
      }
    }
  }
}

bool Mine::check_angle(double angle1, double angle2) const {
  //BAD
  angle1 = (angle2 == 0 && angle1 > 345) ? angle1 - 360 : angle1;
  if (angle1 <= angle2 + 8 && angle1 >= angle2 - 8) {
    return true;
  }
  return false;
}

int Mine::get_image_width() const {
  return width_;
}

int Mine::get_image_height() const {
  return height_;
}

bool Mine::is_alive() const {
  return alive_;
}
