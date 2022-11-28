#ifndef _GAMEENGINE_HPP_

#define _GAMEENGINE_HPP_

#include <limits>
#include <random>

#include "./Constants.hpp"
#include "./GameState.hpp"

namespace hammurabi {
class GameEngine final {
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
  int AcceptIntegerInput(int min = 0, int max = INT_MAX);
  int GetInRangeValue(int, int);
  double GetInRangeValue(double, double);
  bool AcceptYesNoInput();

  GameState state_;
  std::mt19937 gen_;
};
}  // namespace hammurabi

#endif  // _GAMEENGINE_HPP_
