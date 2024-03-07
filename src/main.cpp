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

bool loadMedia();
void close_local_textures();
bool process_key(SDL_Event&, Ship&, enemy enemy_arr[]);
//void init_enemy(enemy& enemy_data, double angle);
//void reinit_enemy(enemy&);
bool enemy_move(enemy& enemy_data, Planet& pl);
void shoot(Ship& sd, enemy enemy_arr[]);
void detect_collision(Ship& ship_data, enemy meteor_arr[]);
void add_life(Planet& pl, Ship& sd);
void calc_cooldown(Ship&);
void init_color(SDL_Color& cl, int R, int G, int B);
bool game_is_running(const Ship&, const Planet&);
int shoot_animation(const Ship&);
void spawn_health();

Render_pipe rp;

//LTexture gEnemyTextures[NUM_ENEMY_TEXTURES];
LTexture gBackground;

/*
 * General game process:
 * 1. Create render pipe, which initialize SDL subsystems
 * 2. Create UI object, which tracks textures and helps render UI
 * 3. Create other game objects, such as: ship, enemy, health_obj, planet
 */
int main(int argc, char* args[]) {
  srand(time(0));
  if (!rp.init()) {
    std::cout << "Failed to init!\n";
    exit(EXIT_FAILURE);
  }
  //Class UI
  UI ui(rp);
  for (int i = 0; i < NUM_ENEMY_TEXTURES; ++i) {
    if (!gEnemyTextures[i].loadFromFile(rp, FILE_PATHS_ENEMY[i])) {
      std::cout << "Failed to load enemy texture!\n";
      exit(EXIT_FAILURE);
    }
  }

  if (!gBackground.loadFromFile(rp, FILE_PATH_BACKGROUND)) {
    std::cout << "Failed to load planet!\n";
    exit(EXIT_FAILURE);
  }
  
  SDL_Event e;
  bool quit = false;

  Ship sd(rp);
  Planet pl;
  Obj_health oh(ui.get_texture(UI::IMAGES::RED_HEART));
  //Enemy 
  UI_killbar uk(rp);

  /*
  for (int i = 0; i < NUM_ENEMY_ON_MAP; ++i) {
    init_enemy(meteor_arr[i], 15 * i);
  }
  */
  //Array of enemies
  Enemy* meteor_arr = new Enemy[NUM_ENEMY_ON_MAP];
  for (int i = 0; i < NUM_ENEMY_ON_MAP; ++i) {
    meteor_arr[i] = Enemy(ui.get_enemy_texture(UI::ENEMY::METEOR), 15 * i);
  }

  //Game loop
  while (!quit && game_is_running(sd, pl)) {
    while (SDL_PollEvent(&e) != 0) {
      quit = process_key(e, sd, meteor_arr);
    }
    //Clear screen
    SDL_SetRenderDrawColor(rp.get_renderer(), 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(rp.get_renderer());

    //Background
    gBackground.render(rp, 0, 0);

    //Hearts planet
    ui.render_planet_health(rp, pl);

    //Hearts ship
    ui.render_ship_health(rp, sd);

    //Bullets
    ui.render_ship_bullets(rp, sd);

    //Draw obj_health
    oh.render(rp);

    //Draw ship
    sd.render(rp);

    //Draw enemy
    for (int i = 0; i < NUM_ENEMY_ON_MAP; ++i) {
      if (enemy_move(meteor_arr[i], pl)) {
        gEnemyTextures[0].render(
          rp,
          meteor_arr[i].x_pos, meteor_arr[i].y_pos,
          nullptr, meteor_arr[i].rd
        );
      }
    }

    //Render text
    uk.render(rp, sd);

    //Process final render
    SDL_RenderPresent(rp.get_renderer());
    
    //Calculate game events
    detect_collision(sd, meteor_arr);
    calc_cooldown(sd);
    oh.calc_spawn();
    if (oh.detect_collision(sd)) {
      add_life(pl, sd);
    }
  }
  delete[] meteor_arr;
  close_local_textures();
  return 0;
}

bool process_key(SDL_Event& e, Ship& sd, enemy enemy_arr[]) {
  static const int MOVE_LEN = 30;
  if (e.type == SDL_QUIT) return true;
  if (e.type == SDL_KEYDOWN) {
    //Change frame
    switch(e.key.keysym.sym) {
      case SDLK_w: sd.image_ = Ship::STATES::MOVE_FORWARD; break;
      case SDLK_s: sd.image_ = Ship::STATES::MOVE_BACKWARD; break;
      case SDLK_SPACE: sd.image_ = shoot_animation(sd); break;
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
        shoot(sd, enemy_arr);
        break;
    }
  }
  return false;
}

void close_local_textures() {
  for (int i = 0; i < NUM_ENEMY_TEXTURES; ++i) {
    gEnemyTextures[i].free();
  }
  gBackground.free();
}

/*
void init_enemy(enemy& enemy_data, double angle) {
  enemy_data.w = gEnemyTextures[0].get_width();
  enemy_data.h = gEnemyTextures[0].get_height();
  enemy_data.x_pos = SPAWN_ENEMY_X;
  enemy_data.y_pos = (SCREEN_HEIGHT - enemy_data.h) / 2;
  enemy_data.shift_enemy = rand() % ENEMY_SPEED_LEVELS + 1;
  enemy_data.frame_rate = rand() % ENEMY_FRAMERATE_LEVELS + 1;
  enemy_data.current_frame = 0;
  enemy_data.rd.angle = angle;
  enemy_data.rd.center = {SCREEN_WIDTH / 2 - enemy_data.x_pos, enemy_data.h / 2};
  enemy_data.rd.flip = SDL_FLIP_NONE;
  enemy_data.draw = false;
  enemy_data.first_spawn = true;
}
*/

/*
void reinit_enemy(enemy& enemy_data) {
  enemy_data.x_pos = SPAWN_ENEMY_X;
  enemy_data.y_pos = (SCREEN_HEIGHT - enemy_data.h) / 2;
  enemy_data.current_frame = 0;
  enemy_data.rd.center = {SCREEN_WIDTH / 2 - enemy_data.x_pos, enemy_data.h / 2};
  enemy_data.draw = false;
  enemy_data.first_spawn = false;
}
*/

bool enemy_move(enemy& ed, Planet& pl) {
  if (std::abs(SCREEN_WIDTH / 2 - ed.x_pos - ed.w / 4) <= PLANET_HITBOX &&
    std::abs(SCREEN_HEIGHT / 2 - ed.y_pos - ed.h / 4) <= PLANET_HITBOX) {
    reinit_enemy(ed); //reinit enemy
    pl.dec_lifes();   //decrease lifes
    return false;
  }
  int spawn_chance = ed.first_spawn ? RAND_SPAWN_FIRST : RAND_SPAWN;
  if (!ed.draw && rand() % spawn_chance == 0) {
    ed.draw = true;
    return false;
  }
  if (ed.draw == true && ed.current_frame % ed.frame_rate == 0) {
    ed.x_pos += ed.shift_enemy;
    ed.rd.center.x -= ed.shift_enemy;
  }
  ++ed.current_frame;
  return true;
}

void shoot(Ship& sd, enemy enemy_arr[]) {
  if (sd.curr_bullets_ <= 0) return;
  --sd.curr_bullets_;
  for (int i = 0; i < NUM_ENEMY_ON_MAP; ++i) {
    if (enemy_arr[i].draw && 
      (eu_mod(static_cast<int>(sd.render_.angle) + COORDS_SYNC, DEGREES_IN_CIRCLE)) == enemy_arr[i].rd.angle) {
      reinit_enemy(enemy_arr[i]);
      ++sd.kills_;
    }
  }
}

void detect_collision(Ship& sd, enemy ma[]) {
  //Wrong, should be 1/2
  int wh_diff = (SCREEN_WIDTH - SCREEN_HEIGHT) / 2;
  //int spawn_diff = -SPAWN_ENEMY_X;
  int spawn_diff = 0;
  int coords_sync = spawn_diff - wh_diff;
  int angle_sync = sd.render_.angle + COORDS_SYNC;

  for (int i = 0; i < NUM_ENEMY_ON_MAP; ++i) {
    if (sd.y_pos_ <= SCREEN_HEIGHT / 2) {
      if (eu_mod(angle_sync, 360) == eu_mod(ma[i].rd.angle, 360)) {
        if (std::abs(sd.y_pos_ - (ma[i].x_pos + coords_sync)) <= SHIP_HITBOX) {
          reinit_enemy(ma[i]);
          --sd.curr_lifes_;
        }
      }
    } else {
      if (eu_mod(angle_sync, 360) != eu_mod(ma[i].rd.angle, 360) &&
          eu_mod(angle_sync, 180) == eu_mod(ma[i].rd.angle, 180)) {
        if (std::abs(-sd.y_pos_ - sd.height_ + SCREEN_HEIGHT - (ma[i].x_pos + coords_sync)) <=
            SHIP_HITBOX - 30) {
          reinit_enemy(ma[i]);
          --sd.curr_lifes_;
        }
      }
    }
  }
}

void add_life(Planet& pl, Ship& sd) {
  if (pl.get_curr_lifes() < pl.get_max_lifes()) pl.inc_lifes();
  else if (sd.curr_lifes_ < sd.max_lifes_) ++sd.curr_lifes_;
}

int shoot_animation(const Ship& sd) {
  return (sd.curr_bullets_ <= 0) ? Ship::STATES::RELOAD : Ship::STATES::SHOOT;
}

void calc_cooldown(Ship& sd) { 
  if (sd.curr_bullets_ > 0) return;
  ++sd.cooldown_timer_;
  if (sd.cooldown_timer_ >= sd.cooldown_) {
    sd.cooldown_timer_ = 0;
    sd.curr_bullets_ = sd.max_bullets_;
  }
}

void init_color(SDL_Color& cl, int r, int g, int b) {
  cl.r = r;
  cl.g = g;
  cl.b = b;
}

bool game_is_running(const Ship& sd, const Planet& pl) {
  return sd.is_fighting() && pl.get_curr_lifes();
}
