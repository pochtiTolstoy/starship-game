#include "util/util.h"
#include "util/constants.h"
#include "util/render_pipe.h"
#include "texture/LTexture.h"
#include "entity/game_controls.h"
#include "entity/ship.h"
#include "entity/planet.h"
#include "entity/ui.h"
#include "entity/obj_health.h"
#include "entity/ui_killbar.h"
#include "entity/enemy.h"
#include <sstream>

const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

void process_key(SDL_Event&, Ship&, Enemy* enemy_arr);
void add_life(Planet& pl, Ship& sd);
bool game_is_running(const Ship&, const Planet&);

class LTimer {
public:
  LTimer();
  void start();
  void stop();
  void pause();
  void unpause();
  Uint32 getTicks();
  bool isStarted();
  bool isPaused();
private:
  Uint32 mStartTicks;
  Uint32 mPausedTicks;
  bool mPaused;
  bool mStarted;
};

LTimer::LTimer() {
  mStartTicks = 0;
  mPausedTicks = 0;
  mPaused = false;
  mStarted = false;
}

void LTimer::start() {
  mStarted = true;
  mPaused = false;
  mStartTicks = SDL_GetTicks();
  mPausedTicks = 0;
}

void LTimer::stop() {
  mStarted = false;
  mPaused = false;
  mStartTicks = 0;
  mPausedTicks = 0;
}

void LTimer::pause() {
  if (mStarted && !mPaused) {
    mPaused = true;
    mPausedTicks = SDL_GetTicks() - mStartTicks;
    mStartTicks = 0;
  }
}

void LTimer::unpause() {
  if (mStarted && mPaused) {
    mPaused = false;
    mStartTicks = SDL_GetTicks() - mPausedTicks;
    mPausedTicks = 0;
  }
}

Uint32 LTimer::getTicks() {
  Uint32 time = 0;
  if (mStarted) {
    if (mPaused) time = mPausedTicks;
    else time = SDL_GetTicks() - mStartTicks;
  }
  return time;
}

bool LTimer::isStarted()
{
	//Timer is running and paused or unpaused
    return mStarted;
}

bool LTimer::isPaused()
{
	//Timer is running and paused
    return mPaused && mStarted;
}

/*
 * General game process:
 * 1. Create render pipe, which initialize SDL subsystems
 * 2. Create UI object, which tracks textures and helps render UI
 * 3. Create other game objects, such as: ship, enemy, health_obj, planet
 */

LTimer angTimer;
LTimer moveTimer;
int count_diff = 0;
const int ang_fix = 3;
int main(int argc, char* args[]) {
  srand(time(0));
  Render_pipe rp;
  if (!rp.init()) {
    std::cout << "Failed to init!\n";
    exit(EXIT_FAILURE);
  }
  //Class UI
  UI ui(rp);

  Ship sd(rp);
  Planet pl;
  Obj_health oh1(ui.get_ui_texture(UI::IMAGES::RED_HEART));
  Obj_health oh2(ui.get_ui_texture(UI::IMAGES::RED_HEART));
  UI_killbar uk(rp);

  //Array of enemies
  Enemy meteor_arr[NUM_ENEMY_ON_MAP];
  for (int i = 0; i < NUM_ENEMY_ON_MAP; ++i) {
    meteor_arr[i].set_angle(i * 15);
    meteor_arr[i].set_texture(ui.get_enemy_texture(UI::ENEMY::METEOR));
  }
  
  SDL_Event e;
  bool quit = false;
  //Test FPS
  LTimer fpsTimer;
  LTimer capTimer;
  SDL_Color textColor = { 0, 0, 0, 255 };
  LTexture gFPSTextTexture;
  int countedFrames = 0;
  std::stringstream timeText;
  //Game loop
  fpsTimer.start();
  while (!quit && game_is_running(sd, pl)) {
    capTimer.start();
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) quit = true;
      process_key(e, sd, meteor_arr);
    }

    //Calculate average FPS
    float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.0f);
    if (avgFPS > 2'000'000) avgFPS = 0;
    timeText.str("");
    timeText << "FPS: " << avgFPS;
    if (!gFPSTextTexture.loadFromRenderedText(
      rp, timeText.str().c_str(), textColor
    )) {
      std::cout << "Unable to render FPS texture!\n";
    }

    //sd.move();
    if (angTimer.isStarted()) {
      if (sd.vel_ang_ < 0)
        sd.render_.angle -= ang_fix;
      else
        sd.render_.angle += ang_fix;
    }
    if (moveTimer.isStarted()) {
      if (sd.vel_r_ < 0) {
        sd.y_pos_ -= 8;
        sd.render_.center.y += 8;
      } else {
        sd.y_pos_ += 8;
        sd.render_.center.y -= 8;
      }
    }
    //Clear screen
    SDL_SetRenderDrawColor(rp.get_renderer(), 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(rp.get_renderer());

    //Background
    ui.render_background(rp);

    //Hearts planet
    ui.render_planet_health(rp, pl);

    //Hearts ship
    ui.render_ship_health(rp, sd);

    //Bullets
    ui.render_ship_bullets(rp, sd);

    //Draw obj_health
    oh1.render(rp);
    oh2.render(rp);

    sd.render(rp);

    //Draw enemy
    for (int i = 0; i < NUM_ENEMY_ON_MAP; ++i) {
      if (meteor_arr[i].detect_planet_collision(pl)) pl.dec_lifes(); 
      if (meteor_arr[i].move()) meteor_arr[i].render(rp);
    }

    //Render text
    uk.render(rp, sd);

    //avg FPS render
    gFPSTextTexture.render(
      rp,
      (SCREEN_WIDTH - gFPSTextTexture.get_width()) / 2,
      0
    );

    //Process final render
    SDL_RenderPresent(rp.get_renderer());
    
    //Calculate game events
    sd.detect_collision(meteor_arr);
    sd.calc_cooldown();
    oh1.calc_spawn();
    oh2.calc_spawn();
    if (oh1.detect_collision(sd)) add_life(pl, sd);
    if (oh2.detect_collision(sd)) add_life(pl, sd);

    ++countedFrames;
    int frameTicks = capTimer.getTicks();
    if (frameTicks < SCREEN_TICK_PER_FRAME) {
      SDL_Delay(SCREEN_TICK_PER_FRAME - frameTicks);
    }
  }
  std::cout << "KILLS: " << sd.kills_ << "/" << KILLS_TO_WIN << '\n';
  return 0;
}

void process_key(SDL_Event& e, Ship& sd, Enemy* enemy_arr) {
  static const int MOVE_LEN = 30;
  int rem = eu_mod(static_cast<int>(sd.render_.angle), 
                   static_cast<int>(MOVE_ANGULAR));
  if (e.type == SDL_KEYDOWN) {
    switch(e.key.keysym.sym) {
      case SDLK_w: sd.image_ = Ship::STATES::MOVE_FORWARD; break;
      case SDLK_s: sd.image_ = Ship::STATES::MOVE_BACKWARD; break;
      case SDLK_SPACE: sd.change_shoot_animation(); break;
      case SDLK_a: 
        sd.image_ = (sd.image_ == Ship::STATES::RELOAD) ? 
                    Ship::STATES::RELOAD : Ship::STATES::DEFAULT;
        break;
      case SDLK_d:
        sd.image_ = (sd.image_ == Ship::STATES::RELOAD) ? 
                    Ship::STATES::RELOAD : Ship::STATES::DEFAULT;
        break;
      default:
        sd.image_ = Ship::STATES::DEFAULT;
    }
  }
  if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
    switch (e.key.keysym.sym) {
      case SDLK_w: 
        sd.vel_r_ -= MOVE_LEN; 
        moveTimer.start();
        break;
      case SDLK_s: 
        sd.vel_r_ += MOVE_LEN; 
        moveTimer.start();
        break;
      case SDLK_a: 
        sd.vel_ang_ -= MOVE_ANGULAR; 
        angTimer.start(); 
        break;
      case SDLK_d: 
        sd.vel_ang_ += MOVE_ANGULAR; 
        angTimer.start(); 
        break;
      //OK
      case SDLK_e: sd.render_.angle += DEGREES_IN_HALF_CIRCLE; break;
      case SDLK_SPACE: sd.shoot(enemy_arr); break;
    }
  } else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
    switch (e.key.keysym.sym) {
      case SDLK_w: 
        sd.vel_r_ += MOVE_LEN; 
        moveTimer.stop();
        break;
      case SDLK_s: 
        sd.vel_r_ -= MOVE_LEN; 
        moveTimer.stop();
        break;
      case SDLK_a: 
        sd.vel_ang_ += MOVE_ANGULAR;
        if (rem > 9)
          //DAMPER CLOCKWISE
          sd.render_.angle += (MOVE_ANGULAR - rem);
        else
          sd.render_.angle -= rem;
        count_diff = 0;
        angTimer.stop();
        break;
      case SDLK_d:
        sd.vel_ang_ -= MOVE_ANGULAR;
        if (rem > 5)
          sd.render_.angle += (MOVE_ANGULAR - rem);
        else
          //DAMPER COUNTER-CLOCKWISE
          sd.render_.angle -= rem;
        count_diff = 0;
        angTimer.stop();
        break;
    }
  }
}

void add_life(Planet& pl, Ship& sd) {
  if (pl.get_curr_lifes() < pl.get_max_lifes()) pl.inc_lifes();
  else if (sd.curr_lifes_ < sd.max_lifes_) ++sd.curr_lifes_;
}

bool game_is_running(const Ship& sd, const Planet& pl) {
  return sd.is_fighting() && pl.get_curr_lifes();
}
