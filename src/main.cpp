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

bool process_key(SDL_Event&, Ship&, Enemy* enemy_arr);
void add_life(Planet& pl, Ship& sd);
bool game_is_running(const Ship&, const Planet&);

/*
 * General game process:
 * 1. Create render pipe, which initialize SDL subsystems
 * 2. Create UI object, which tracks textures and helps render UI
 * 3. Create other game objects, such as: ship, enemy, health_obj, planet
 */
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
    //meteor_arr[i] = Enemy(ui.get_enemy_texture(UI::ENEMY::METEOR), 15 * i);
    meteor_arr[i].set_angle(i * 15);
    meteor_arr[i].set_texture(ui.get_enemy_texture(UI::ENEMY::METEOR));
  }
  
  SDL_Event e;
  bool quit = false;

  //Game loop
  while (!quit && game_is_running(sd, pl)) {
    while (SDL_PollEvent(&e) != 0) {
      quit = process_key(e, sd, meteor_arr);
    }
    //Clear screen
    SDL_SetRenderDrawColor(rp.get_renderer(), 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(rp.get_renderer());

    //Background
    //gBackground.render(rp, 0, 0);
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

    //Draw ship
    sd.render(rp);

    //Draw enemy
    for (int i = 0; i < NUM_ENEMY_ON_MAP; ++i) {
      if (meteor_arr[i].detect_planet_collision(pl)) pl.dec_lifes(); 
      if (meteor_arr[i].move()) meteor_arr[i].render(rp);
    }

    //Render text
    uk.render(rp, sd);

    //Process final render
    SDL_RenderPresent(rp.get_renderer());
    
    //Calculate game events
    sd.detect_collision(meteor_arr);
    sd.calc_cooldown();
    oh1.calc_spawn();
    oh2.calc_spawn();
    if (oh1.detect_collision(sd)) add_life(pl, sd);
    if (oh2.detect_collision(sd)) add_life(pl, sd);
  }
  //close_local_textures();
  return 0;
}

bool process_key(SDL_Event& e, Ship& sd, Enemy* enemy_arr) {
  static const int MOVE_LEN = 30;
  if (e.type == SDL_QUIT) return true;
  if (e.type == SDL_KEYDOWN) {
    //Change frame
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

    //Change move
    switch(e.key.keysym.sym) {
      case SDLK_a: sd.render_.angle -= MOVE_ANGULAR;
        break;
      case SDLK_d: sd.render_.angle += MOVE_ANGULAR;
        break;
      case SDLK_w: 
        sd.y_pos_ -= MOVE_LEN;
        sd.render_.center.y += MOVE_LEN;
        break;
      case SDLK_s: 
        sd.y_pos_ += MOVE_LEN;
        sd.render_.center.y -= MOVE_LEN;
        break;
      case SDLK_e:
        sd.render_.angle += DEGREES_IN_HALF_CIRCLE;
        break;
      case SDLK_SPACE: 
        sd.shoot(enemy_arr);
        break;
    }
  }
  return false;
}

void add_life(Planet& pl, Ship& sd) {
  if (pl.get_curr_lifes() < pl.get_max_lifes()) pl.inc_lifes();
  else if (sd.curr_lifes_ < sd.max_lifes_) ++sd.curr_lifes_;
}

bool game_is_running(const Ship& sd, const Planet& pl) {
  return sd.is_fighting() && pl.get_curr_lifes();
}
