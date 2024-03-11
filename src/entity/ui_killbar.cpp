#include "ui_killbar.h"

UI_killbar::UI_killbar(Render_pipe& rp)
  : max_kills_(KILLS_TO_WIN), curr_kills_(0) 
{
  text_ = "Kills: 0/" + std::to_string(max_kills_);
  color_ = { 0, 0, 0 };
  gTextTexture_.loadFromRenderedText(rp, text_, color_);
}

UI_killbar::~UI_killbar() {
  gTextTexture_.free();
}

void UI_killbar::render(Render_pipe& rp, const Ship& sd) {
  //std::cout << "SHIP KILLS: " << sd.kills_ << ", UK KILLS: " << curr_kills_ << '\n';
  if (curr_kills_ != sd.kills_) {
    curr_kills_ = sd.kills_;
    text_ = "Kills: " + std::to_string(curr_kills_) +
                  "/" + std::to_string(KILLS_TO_WIN);
    gTextTexture_.loadFromRenderedText(rp, text_, color_);
  }
  gTextTexture_.render(
    rp,
    (SCREEN_WIDTH - gTextTexture_.get_width()) / 2,
    SCREEN_HEIGHT - gTextTexture_.get_height()
  );
}
