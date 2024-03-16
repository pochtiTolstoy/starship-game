#include "obj_health.h"

Obj_health::Obj_health(const LTexture& t): gHealthTexture_(t) {
  //Init linear data
  width_  = t.get_width();
  height_ = t.get_height();
  x_pos_  = (SCREEN_WIDTH - width_) / 2;
  y_pos_  = (SCREEN_HEIGHT - height_) / 2 - 500;
  //Init rotation data
  render_.angle  = 0; 
  render_.center = { width_ / 2, height_ / 2 + 500};
  render_.flip   = SDL_FLIP_NONE;

  //Init condition
  draw_ = false;
}

Obj_health::~Obj_health() {}

void Obj_health::render(Render_pipe& rp) {
  if (!draw_) return;
  //render_.angle += 1;
  gHealthTexture_.render(
    rp,
    x_pos_, y_pos_,
    nullptr, render_
  );
}

void Obj_health::calc_spawn() {
  if (draw_ || rand() % 100 != 0) return;
  //Turn off after collision with ship
  draw_ = true;
  render_.angle = (rand() % 24) * 15;
  //render_.angle = ;
  if (check_angle())
    y_pos_ = rand() % 50;
  else y_pos_ = rand() % 200 - 400; 
  int mid = y_pos_ + height_ / 2;
  render_.center = { width_ / 2, SCREEN_HEIGHT / 2 - y_pos_ };
}

bool Obj_health::detect_collision(const Ship& sd) {
  if (!draw_) return false;
  //int coords_sync = (SCREEN_WIDTH - SCREEN_HEIGHT) / 2;
  //int angle_sync = sd.render_.angle + COORDS_SYNC;
  int mid_ship_y = sd.y_pos_ + sd.height_ / 2 - 40;
  int mid_health_y = y_pos_ + height_ / 2;
  if (sd.y_pos_ <= SCREEN_HEIGHT / 2) {
    if (eu_mod(sd.render_.angle, 360) == eu_mod(render_.angle, 360)) {
      if (std::abs(mid_ship_y - mid_health_y) <= 25) {
        draw_ = false; 
        return true; 
      }
    }
    return false;
  }
  int reflection_y;
  if ((eu_mod(sd.render_.angle, 360) != eu_mod(render_.angle, 360)) &&
      (eu_mod(sd.render_.angle, 180) == eu_mod(render_.angle, 180))) {
    reflection_y = -sd.y_pos_ - sd.height_ / 2 + SCREEN_HEIGHT;
    if (std::abs(reflection_y - mid_health_y) <= 30) {
      draw_ = false;
      return true;
    }
  }
  return false;
}

bool Obj_health::check_angle() const {
  return (render_.angle >= 0  && render_.angle <= 45) ||
         (render_.angle >= 135 && render_.angle <= 225 ||
         (render_.angle >= 315 && render_.angle <= 360));
}

bool Obj_health::is_alive() const {
  return draw_;
}

const r_data& Obj_health::get_render_data() const {
  return render_;
}

double Obj_health::get_angle() const {
  return render_.angle;
}

void Obj_health::kill() {
  draw_ = false;
}

int Obj_health::get_y_pos() const {
  return y_pos_;
}

int Obj_health::get_image_height() const {
  return height_;
}
