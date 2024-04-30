#include "planet.h"

Planet::Planet(int curr_lifes, int max_lifes)
  : curr_lifes_(curr_lifes), max_lifes_(max_lifes) {}

Planet::~Planet() {}

void Planet::inc_lifes() { ++curr_lifes_; }

void Planet::dec_lifes() { /*--curr_lifes_;*/ }

//Getters
int Planet::get_max_lifes() const { return max_lifes_; }
int Planet::get_curr_lifes() const { return curr_lifes_; }
