#include "ship.h"

//Default constructor
Ship::Ship(Render_pipe& rp, int max_lifes, int max_bullets, int cooldown)
  : image_(STATES::DEFAULT), kills_(0),
    max_lifes_(max_lifes),        curr_lifes_(max_lifes),
    max_bullets_(max_bullets),    curr_bullets_(max_bullets),
    cooldown_(cooldown),          cooldown_timer_(0)
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
