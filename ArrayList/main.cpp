#include "ArrayList.hpp"

#include <iostream>


int main() {
  ArrayList<int> a;

  for (int i = 0; i < 10; ++i) {
    a.Insert(i);
  }

  for (auto it = a.iterator(); it.HasNext(); it.Next()) {
    std::cout << it.Get() << std::endl;
  }

  for (auto it = a.reverseIterator(); it.HasNext(); it.Next()) {
    std::cout << it.Get() << std::endl;
  }
}
