#include <iostream>
#include <string>

#include "ArrayList.hpp"

int main() {
  ArrayList<std::string> a,b;

  for (int i = 0; i < 10; ++i) {
    a.Insert("aaa" + std::to_string(i));
  }

  b = std::move(a);

  for (const auto& e : b) {
    std::cout << e << std::endl;
  }
}
