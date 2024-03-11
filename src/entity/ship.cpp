#include "ship.h"

//Default constructor
Ship::Ship(Render_pipe& rp, int max_lifes, int max_bullets, int cooldown)
  : vel_r_(0), vel_ang_(0), moving_r_(false), moving_ang_(false),
    curr_lifes_(max_lifes), max_lifes_(max_lifes),
    max_bullets_(max_bullets),    curr_bullets_(max_bullets),
    cooldown_(cooldown),          cooldown_timer_(0),
    kills_(0), image_(STATES::DEFAULT)
{
  init_images(rp);
  width_  = get_image_width(STATES::DEFAULT);
  height_ = get_image_height(STATES::DEFAULT);
  x_pos_  = (SCREEN_WIDTH  - width_ ) / 2;
  y_pos_  = (SCREEN_HEIGHT - height_) / 2;
  render_.angle  = 0;
  render_.center = { width_ / 2, height_ / 2 };
  render_.flip   = SDL_FLIP_NONE;
}

//Destructor
Ship::~Ship() {
  for (int i = 0; i < NUM_SHIP_TEXTURES; ++i) {
    gShipTextures_[i].free();
  }
}

//Public methods
void Ship::render(Render_pipe& rp) {
  if (is_reloaded()) image_ = STATES::DEFAULT;
  if (is_image_high()) render_high_image(rp);
  else render_image(rp);
}

/*
void Ship::move() {
  render_.angle += vel_ang_;
}
*/

void Ship::shoot(Enemy* enemy_arr) {
  if (curr_bullets_ <= 0) return;
  --curr_bullets_;
  //Triple gun
  /*
  for (int i = 0; i < NUM_ENEMY_ON_MAP; ++i) {
    if (is_angle_sync(render_.angle - 15, enemy_arr[i])) {
      enemy_arr[i].reinit();
      ++kills_;
    } else if (is_angle_sync(render_.angle, enemy_arr[i])) {
      enemy_arr[i].reinit();
      ++kills_;
    } else if (is_angle_sync(render_.angle + 15, enemy_arr[i])) {
      enemy_arr[i].reinit();
      ++kills_;
      return;
    }
  }
  */
  for (int i = 0; i < NUM_ENEMY_ON_MAP; ++i) {
    if (is_angle_sync(render_.angle, enemy_arr[i])) {
      enemy_arr[i].reinit();
      ++kills_;
      return;
    }
  }
}

void Ship::calc_cooldown() {
  if (curr_bullets_ > 0) return;
  ++cooldown_timer_;
  if (cooldown_timer_ >= cooldown_) {
    cooldown_timer_ = 0;
    curr_bullets_ = max_bullets_;
  }
}

void Ship::detect_collision(Enemy* e) {
  int coords_sync = (SCREEN_WIDTH - SCREEN_HEIGHT) / 2;
  int angle_sync = render_.angle + COORDS_SYNC; 
  int diff = SCREEN_HEIGHT - y_pos_ - height_ + coords_sync;
  for (int i = 0; i < NUM_ENEMY_ON_MAP; ++i) {
    if (y_pos_ <= SCREEN_HEIGHT / 2) {
      if (eu_mod(angle_sync, 360) == eu_mod(e[i].get_angle(), 360)) {
        //std::cout << "SHIP: " << y_pos_ << ", e[i] - dx: "
          //<< e[i].get_x() - coords_sync << '\n';
        if (std::abs(y_pos_ - e[i].get_x() + coords_sync) <= SHIP_HITBOX) {
          e[i].reinit();
          --curr_lifes_;
        }
      }
    } else {
      if (eu_mod(angle_sync, 360) != eu_mod(e[i].get_angle(), 360) &&
          eu_mod(angle_sync, 180) == eu_mod(e[i].get_angle(), 180)) {
        if (std::abs(diff - e[i].get_x()) <= SHIP_HITBOX - 30) {
          e[i].reinit();
          --curr_lifes_;
        }
      }
    }
  }
}

void Ship::change_shoot_animation() {
  image_ = (curr_bullets_ <= 0) ? STATES::RELOAD : STATES::SHOOT;
}

bool Ship::is_fighting() const {
  return curr_lifes_ && kills_ < KILLS_TO_WIN;
}

//======================Helper methods===========================
const SDL_Color& Ship::get_cut_color(int image) const {
  static const SDL_Color white = { 0xFF, 0xFF, 0xFF };
  static const SDL_Color black = { 0x00, 0x00, 0x00 };
  switch (image)  {
    case STATES::RELOAD: return black;
    default:             return white;
  }
}

bool Ship::is_reloaded() const {
  return image_ == STATES::RELOAD && curr_bullets_ > 0;
}

/**
 * Answers if current image of ship 
 * is higher than DEFAULT image height parameter
 */
bool Ship::is_image_high() const {
  return get_image_height(STATES::DEFAULT) < get_image_height(image_);
}

void Ship::render_image(Render_pipe& rp) const {
  gShipTextures_[image_].render(
    rp,
    x_pos_, y_pos_, nullptr, render_
  );
}

void Ship::render_high_image(Render_pipe& rp) const {
  gShipTextures_[image_].render(
    rp,
    x_pos_, calc_high_y(), nullptr, calc_rotation_high()
  );
}

void Ship::init_images(Render_pipe& rp) {
  SDL_Color cut_color;
  for (int i = 0; i < NUM_SHIP_TEXTURES; ++i) {
    cut_color = get_cut_color(i);
    if (!gShipTextures_[i].loadFromFile(rp, FILE_PATHS_SHIP[i], cut_color)) {
      std::cerr << "Failed to load ship texture!\n"; 
      exit(EXIT_FAILURE);
      // write try catch or something instead of exit
    }
  }
}

int Ship::calc_high_y() const {
  return y_pos_ - get_image_height(image_) + height_;
}

r_data Ship::calc_rotation_high() const {
  r_data rd = render_;
  rd.center.y += get_image_height(image_) - height_;
  return rd;
}

int Ship::get_image_height(int image) const {
  return gShipTextures_[image].get_height();
}

int Ship::get_image_width(int image) const {
  return gShipTextures_[image].get_width();
}

bool Ship::is_angle_sync(double angle, const Enemy& enemy) {
  int angle_sync = static_cast<int>(angle) + COORDS_SYNC;
  return enemy.is_alive() && eu_mod(angle_sync, 360) == enemy.get_angle();
}
