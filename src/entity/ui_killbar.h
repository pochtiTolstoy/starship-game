#ifndef UI_KILLBAR_H_
#define UI_KILLBAR_H_

#include "../texture/LTexture.h"
#include "../util/render_pipe.h"
#include "../util/constants.h"
#include "ship.h"

class UI_killbar {
public:
  UI_killbar() = delete;
  UI_killbar(Render_pipe&);
  ~UI_killbar();
  void render(Render_pipe&, const Ship&);
private:
  LTexture gTextTexture_; 
	int max_kills_;
	int curr_kills_;
	std::string text_;
	SDL_Color color_;
};

#endif /* UI_KILLBAR_H_ */
