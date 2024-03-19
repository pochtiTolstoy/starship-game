#ifndef BUTTON_H_
#define BUTTON_H_

#include "../util/util.h"
#include "../util/render_pipe.h"
#include "../texture/LTexture.h"

class Button {
public:
  Button() = delete; 
  Button(
    Render_pipe&,
    const std::string&,
    const SDL_Color&,
    const SDL_Color&,
    int offset = 4
  );
  ~Button();
  void update_text(
    Render_pipe& rp, 
    const std::string& text, 
    const SDL_Color&,
    const SDL_Color&
  );
  int get_width() const;
  int get_height() const;
  void render(Render_pipe&) const;
  void set_position(int x_pos, int y_pos);
private:
  LTexture front_text_;
  LTexture shadow_;
  SDL_Color color_front_;
  SDL_Color color_shadow_;
  std::string text_;

  //Position data
  int x_pos_;
  int y_pos_;
  int offset_;
};

#endif /* BUTTON_H_ */
