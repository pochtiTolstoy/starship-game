#include "text_box.h"

Text_box::Text_box(Render_pipe &rp, const std::string &text,
                   const SDL_Color &c1, const SDL_Color &c2, int offset,
                   bool long_text, int ind)
    : front_text_(), shadow_(), color_front_(c1), color_shadow_(c2),
      text_(text), x_pos_(-1), y_pos_(-1), offset_(offset), ind_(ind) {
  if (long_text)
    update_long_text(rp, text, color_front_, color_shadow_);
  else
    update_text(rp, text, color_front_, color_shadow_);
}

Text_box::~Text_box() {
  front_text_.free();
  shadow_.free();
}

void Text_box::set_position(int x_pos, int y_pos) {
  x_pos_ = x_pos;
  y_pos_ = y_pos;
}

void Text_box::update_text(Render_pipe &rp, const std::string &text,
                           const SDL_Color &c1, const SDL_Color &c2) {
  if (!front_text_.loadFromRenderedText(rp, text, c1, ind_)) {
    std::cout << "Error: Unable to render text button: " << text << '\n';
    exit(EXIT_FAILURE);
  }
  if (!shadow_.loadFromRenderedText(rp, text, c2, ind_)) {
    std::cout << "Error: Unable to render shadow of button: " << text << '\n';
    exit(EXIT_FAILURE);
  }
}

void Text_box::update_long_text(Render_pipe &rp, const std::string &text,
                                const SDL_Color &c1, const SDL_Color &c2) {
  if (!front_text_.loadFromRenderedLongText(rp, text, c1, ind_)) {
    std::cout << "Error: Unable to render text button: " << text << '\n';
    exit(EXIT_FAILURE);
  }
  if (!shadow_.loadFromRenderedLongText(rp, text, c2, ind_)) {
    std::cout << "Error: Unable to render text button: " << text << '\n';
    exit(EXIT_FAILURE);
  }
}

void Text_box::render(Render_pipe &rp) const {
  if (x_pos_ == -1 || y_pos_ == -1) {
    std::cout << "Warning: set position for your buttons.\n";
    return;
  }
  // Render Shadow
  shadow_.render(rp, x_pos_ + offset_, y_pos_ + offset_);

  // Render front text
  front_text_.render(rp, x_pos_, y_pos_);
}

int Text_box::get_width() const { return front_text_.get_width(); }

int Text_box::get_height() const { return front_text_.get_height(); }
