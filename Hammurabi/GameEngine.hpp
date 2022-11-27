#ifndef _GAMEENGINE_HPP_

#define _GAMEENGINE_HPP_

#include <random>

#include "./Constants.hpp"
#include "./GameState.hpp"

class GameEngine {
 public:
  GameEngine();

  void MainLoop();

 private:
  void StartRound();
  void ShowRoundInfo();
  int Step();
  void GameOver(int result);
  void GameFinished();
  bool CanLoadState();
  void LoadState();
  void SaveState();
  int AcceptInput();
  int GetInRangeValue(int, int);
  double GetInRangeValue(double, double);

  GameState state_;
  std::mt19937 gen_;
};

#endif  // _GAMEENGINE_HPP_
