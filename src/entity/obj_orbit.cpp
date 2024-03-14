#include "obj_orbit.h"

Obj_orbit::Obj_orbit(const LTexture& t): gObjTexture_(t) {
  //Init linear data
  width_  = t.get_width();
  height_ = t.get_height();
  x_pos_  = (SCREEN_WIDTH - width_) / 2;
  y_pos_  = (SCREEN_HEIGHT - height_) / 2;
  //Init rotation data
  render_.angle  = 0; 
  render_.center = { width_ / 2, height_ / 2};
  render_.flip   = SDL_FLIP_NONE;

  //Init condition
  draw_ = false;
}

Obj_orbit::~Obj_orbit() {}

void Obj_orbit::render(Render_pipe& rp) {
  if (!draw_) return;
  gObjTexture_.render(
    rp,
    x_pos_, y_pos_,
    nullptr, render_
  );
}

void Obj_orbit::calc_spawn(const Ship& sd, const Orbit& orb) {
  if (!draw_ && !orb.is_alive() && sd.kills_ && sd.kills_ % 1 == 0) {
    draw_ = true; //Turn off after collision with ship
  } else return;
  render_.angle = (rand() % 24) * 15;
  y_pos_ = rand() % 20 - height_ / 2;
  render_.center = {width_ / 2, SCREEN_HEIGHT / 2 - y_pos_ };
}

/*
bool Obj_orbit::check_angle() const {
  return (render_.angle >= 0  && render_.angle <= 45) ||
         (render_.angle >= 135 && render_.angle <= 225) ||
         (render_.angle >= 315 && render_.angle <= 360);
}
*/

bool Obj_orbit::detect_collision(const Ship& sd) {
  if (!draw_) return false;
  int mid_ship_y = sd.y_pos_ + sd.height_ / 2 - 40;
  int mid_obj_x = y_pos_ + height_ / 2;
  if (sd.y_pos_ <= SCREEN_HEIGHT / 2) {
    if (eu_mod(sd.render_.angle, 360) == eu_mod(render_.angle, 360)) {
      if (std::abs(mid_ship_y - mid_obj_x) <= 25) {
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
    if (std::abs(reflection_y - mid_obj_x) <= 30) {
      draw_ = false;
      return true;
    }
  }
  return false;
}
