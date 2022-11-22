#include <iostream>
#include <string>

#include "ArrayList.hpp"

int main() {
  ArrayList<std::string> a;

  for (int i = 0; i < 10; ++i) {
    a.Insert("aaa" + std::to_string(i));
  }

  for (const auto& e : a) {
    std::cout << e << std::endl;
  }
}
