#ifndef _GAMESTATE_HPP_

#define _GAMESTATE_HPP_

#include <nlohmann/json.hpp>

#include "./Constants.hpp"

namespace hammurabi {
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
  int wheat_eaten_by_rats = 0;
  int wheat_stash = kStartWheatStash;
  int acreage = kStartAcreage;
  int acre_cost = 0;
  int cumulative_starvation_death_per_year = 0;
};

// Nlohmann json library built-in solution
// https://github.com/nlohmann/json#simplify-your-life-with-macros
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GameState, seed, round, people_dead,
                                   people_arrived, is_plague, population,
                                   acres_planted, wheat_harvested,
                                   wheat_per_acre, wheat_eaten_by_rats,
                                   wheat_stash, acreage, acre_cost);
}  // namespace hammurabi
#endif  // _GAMESTATE_HPP_
