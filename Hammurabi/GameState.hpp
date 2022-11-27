#ifndef _GAMESTATE_HPP_

#define _GAMESTATE_HPP_

#include "./Constants.hpp"

struct GameState {
  unsigned int seed = 0;
  int round = 0;
  int people_dead = 0;
  int people_arrived = 0;
  bool is_plague = false;
  int population = kStartPopulation;
  int acres_planted = 0;
  int wheat_harvested = 0;
  int wheat_per_acre = 0;
  double wheat_eaten_by_rats = 0;
  double wheat_stash = kStartWheatStash;
  int acreage = kStartAcreage;
  int acre_cost = 0;
};

#endif  // _GAMESTATE_HPP_
