#include "ui_killbar.h"

UI_killbar::UI_killbar(Render_pipe& rp)
  : max_kills_(KILLS_TO_WIN), curr_kills_(0) 
{
  text_ = "KILLS: 0/" + std::to_string(max_kills_);
  color_ = { 0, 192, 248, 0xFF };
  color_shadow_ =  { 7, 63, 147, 0xFF };
  gTextTexture_.loadFromRenderedText(rp, text_, color_, 1);
  gShadow_.loadFromRenderedText(rp, text_, color_shadow_, 1);
}

UI_killbar::~UI_killbar() {
  gTextTexture_.free();
  gShadow_.free();
}

void UI_killbar::render(Render_pipe& rp, const Ship& sd) {
  //std::cout << "SHIP KILLS: " << sd.kills_ << ", UK KILLS: " << curr_kills_ << '\n';
  if (curr_kills_ != sd.kills_) {
    curr_kills_ = sd.kills_;
    text_ = "KILLS: " + std::to_string(curr_kills_) +
                  "/" + std::to_string(KILLS_TO_WIN);
    gTextTexture_.loadFromRenderedText(rp, text_, color_, 1);
    gShadow_.loadFromRenderedText(rp, text_, color_shadow_, 1);
  }
  gShadow_.render(
    rp,
    (SCREEN_WIDTH - gShadow_.get_width()) / 2 + 4,
    SCREEN_HEIGHT - gShadow_.get_height() + 4
  );
  gTextTexture_.render(
    rp,
    (SCREEN_WIDTH - gTextTexture_.get_width()) / 2,
    SCREEN_HEIGHT - gTextTexture_.get_height()
  );
}
