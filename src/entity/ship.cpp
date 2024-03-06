#include "ship.h"

//Default constructor
Ship::Ship(int max_lifes = 2, int max_bullets = 6, int cooldown = 100)
  : image_(SHIP_STATES::DEFAULT), shift_ship_(0), 
    max_lifes_(max_lifes),        curr_lifes_(max_lifes),
    max_bullets_(max_bullets),    curr_bullets_(max_bullets),
    cooldown_(cooldown),          cooldown_timer_(0),
    kills_(0) 
{
  SDL_Color cut_color;
  for (int i = 0; i < NUM_SHIP_TEXTURES; ++i) {
    cut_color = get_cut_color(i);
    if (!gShipTextures_[i].loadFromFile(FILE_PATHS_SHIP[i], cut_color)) {
      std::cerr << "Failed to load ship texture!\n"; 
      exit(EXIT_FAILURE);

      // write try catch or something instead of exit
    }
  }
  width_  = gShipTextures_[SHIP_STATES::DEFAULT].get_width();
  height_ = gShipTextures_[SHIP_STATES::DEFAULT].get_height();
  x_pos_  = (SCREEN_WIDTH  - width_ ) / 2;
  y_pox_  = (SCREEN_HEIGHT - height_) / 2;
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
void render() const {
  //BAD: shift_ship
  int y_pos = y_pos_ - shift_ship_;
  if (is_reloaded()) image_ = SHIP_STATES::DEFAULT;
  if (is_image_high()) render_high_image(y_pos);
  else render_image(y_pos);
}

//Helper methods
const SDL_Color& Ship::get_cut_color(int image) const {
  switch (image)  {
    case SHIP_STATES::RELOAD: return { 0x00, 0x00, 0x00 }; break;
    default                   return { 0xFF, 0xFF, 0xFF }; break;
  }
}

bool is_reloaded() const {
  return image_ == SHIP_STATES::RELOAD && curr_bullets_ > 0;
}

/**
 * Answers if current image of ship 
 * is higher than DEFAULT image height parameter
 */
bool is_image_high() {
  return gShipTextures_[SHIP_STATES::DEFAULT].get_height() <
         gShipTextures_[image_].get_height();
}

void render_high_image() const {
  static int y_pos = y_pos_  
}

void render_image() const {
  gShipTextures_[image_].render(x_pos_, y_pos_, nullptr, render_);
}