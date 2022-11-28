#include "./GameEngine.hpp"

using json = nlohmann::json;
using hammurabi::GameEngine;

int main() {
  setlocale(LC_ALL, "Russian");

  GameEngine ge;

  ge.MainLoop();
}
