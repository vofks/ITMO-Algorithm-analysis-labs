#ifndef _STATE_SERIALIZER_HPP_
#define _STATE_SERIALIZER_HPP_
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

#include "./Constants.hpp"
#include "./GameState.hpp"

namespace hammurabi {
namespace serializer {
using json = nlohmann::json;

static void SerializeState(const GameState& state,
                           const std::string& file_name = kDefaultFileName) {
  std::ofstream file;
  file.open(file_name);

  json data = state;

  file << data;
}

static GameState DeserializeState(
    const std::string& file_name = kDefaultFileName) {
  std::ifstream file;
  file.open(file_name);

  json data = json::parse(file);

  return data.get<GameState>();
}
}  // namespace serializer
}  // namespace hammurabi

#endif  // _STATE_SERIALIZER_HPP_