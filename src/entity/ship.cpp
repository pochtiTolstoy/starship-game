#include "ship.h"

//Default constructor
Ship::Ship(Render_pipe& rp, int max_lifes, int max_bullets, int cooldown): 
  vel_r_(0),
  vel_ang_(0),
  moving_r_(false),
  moving_ang_(false),
  curr_lifes_(max_lifes),
  max_lifes_(max_lifes),
  max_bullets_(max_bullets),
  curr_bullets_(max_bullets),
  cooldown_(cooldown),
  cooldown_timer_(),
  kill_streak_(0),
  kills_(0), 
  image_(STATES::DEFAULT),
  gun_state_(GUN_STATES::DEFAULT)
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

void Ship::move(double delta_time) {
  int distance_to_center = std::abs(y_pos_ + 128 - SCREEN_HEIGHT / 2);
  if (moving_ang_) {
    if (vel_ang_ < 0) {
      render_.angle -= std::abs((-0.20 * distance_to_center + 200) * delta_time);
    }
    else {
      render_.angle += std::abs((-0.20 * distance_to_center + 200) * delta_time);
    }
  }
  if (moving_r_) {
    if (vel_r_ < 0) {
      y_pos_ -= std::floor(480 * delta_time);
      render_.center.y += std::floor(480 * delta_time);
    } else {
      y_pos_ += std::floor(480 * delta_time);
      render_.center.y -= std::floor(480 * delta_time);
    }
  }
}

void Ship::process_shooting(Enemy* enemy_arr) {
  if (curr_bullets_ <= 0) return;
  if (gun_state_ == GUN_STATES::DEFAULT &&
      kill_streak_ && kill_streak_ % 10 == 0
  ) {
    gun_state_ = GUN_STATES::TRIPLE;
    max_bullets_ = 4;
    curr_bullets_ = max_bullets_;
  }
  shoot(enemy_arr);
}

void Ship::shoot(Enemy* enemy_arr) {
  --curr_bullets_;
  if (gun_state_ == GUN_STATES::TRIPLE) {
    triple_shoot(enemy_arr);
  } else if (gun_state_ == GUN_STATES::DEFAULT) {
    default_shoot(enemy_arr);
  }
}

void Ship::change_shoot_animation() {
  if (curr_bullets_ <= 0) {
    image_ = STATES::RELOAD;
  } else if (gun_state_ == GUN_STATES::TRIPLE) {
    image_ = STATES::TRIPLE;
  } else if (gun_state_ == GUN_STATES::DEFAULT &&
             kill_streak_ && kill_streak_ % 10 == 0
  ) {
    image_ = STATES::TRIPLE;
  } else if (gun_state_ == GUN_STATES::DEFAULT) {
    image_ = STATES::SHOOT;
  }
  //image_ = (curr_bullets_ <= 0) ? STATES::RELOAD : STATES::SHOOT;
}

void Ship::calc_cooldown() {
  if (curr_bullets_ > 0) return;
  if (gun_state_ != GUN_STATES::DEFAULT) {
    gun_state_ = GUN_STATES::DEFAULT;
    max_bullets_ = 6;
    curr_bullets_ = max_bullets_;
    return;
  } else if (kill_streak_ && kill_streak_ % 10 == 0) {
    gun_state_ = GUN_STATES::TRIPLE;
    max_bullets_ = 4; 
    curr_bullets_ = max_bullets_;
    return;
  }
  if (!cooldown_timer_.isStarted()) {
    cooldown_timer_.start();
  }
  if (cooldown_timer_.getTicks() >= cooldown_) {
    cooldown_timer_.stop();
    curr_bullets_ = max_bullets_;
  }
}

void Ship::detect_collision(Enemy* e) {
  int mid_ship_y = y_pos_ + height_ / 2 - 40;
  int reflection_y = -y_pos_ - height_ / 2 + SCREEN_HEIGHT;
  int mid_enemy_y = 0;
  for (int i = 0; i < NUM_ENEMY_ON_MAP; ++i) {
    if (!e[i].is_alive()) continue;
    mid_enemy_y = e[i].get_y() + e[i].get_height() / 2;
    if (y_pos_ <= SCREEN_HEIGHT / 2) {
      if (eu_mod(render_.angle, 360) == eu_mod(e[i].get_angle(), 360)) {
        if (std::abs(mid_ship_y - mid_enemy_y) <= 75) {
          e[i].reinit();
          --curr_lifes_;
        }
      }
    } else {
      if (eu_mod(render_.angle, 360) != eu_mod(e[i].get_angle(), 360) &&
          eu_mod(render_.angle, 180) == eu_mod(e[i].get_angle(), 180)) {
        if (std::abs(reflection_y - mid_enemy_y) <= 75) {
          e[i].reinit();
          --curr_lifes_;
        }
      }
    }
  }
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
  r_data temp_render = calc_rotation_high();
  temp_render.center.x = get_image_width(image_) / 2;
  int temp_width = (SCREEN_WIDTH - get_image_width(image_)) / 2;

  gShipTextures_[image_].render(
    rp,
    temp_width, calc_high_y(), nullptr, temp_render
  );
}

void Ship::init_images(Render_pipe& rp) {
  SDL_Color cut_color;
  for (int i = 0; i < NUM_SHIP_TEXTURES; ++i) {
    cut_color = get_cut_color(i);
    if (!gShipTextures_[i].loadFromFile(rp, FILE_PATHS_SHIP[i], cut_color)) {
      std::cerr << "Failed to load ship texture!\n"; 
      exit(EXIT_FAILURE);
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
  return enemy.is_alive() && eu_mod(angle, 360) == enemy.get_angle();
}

void Ship::default_shoot(Enemy* enemy_arr) {
  for (int i = 0; i < NUM_ENEMY_ON_MAP; ++i) {
    if (is_angle_sync(render_.angle, enemy_arr[i])) {
      enemy_arr[i].reinit();
      ++kills_;
      ++kill_streak_;
      return;
    }
  }
  kill_streak_ = 0;
}

void Ship::triple_shoot(Enemy* enemy_arr) {
  //BAD
  int old_kills = kills_;
  for (int i = 0; i < NUM_ENEMY_ON_MAP; ++i) {
    for (int mov_ang = -15; mov_ang <= 15; mov_ang += 15) {
      if (is_angle_sync(render_.angle + mov_ang, enemy_arr[i])) {
        enemy_arr[i].reinit();
        ++kills_;
      } 
    }
  }
  kill_streak_ = (kills_ != old_kills) ? (kill_streak_ + 1) : 0;
}
