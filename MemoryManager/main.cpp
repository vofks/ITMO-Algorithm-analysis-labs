#include <cstdint>
#include <cstdlib>
#include <iostream>

#include "./FixedAllocator.hpp"

int main() {
  memory_manager::FixedAllocator alloc(16, 10);
  alloc.Init();
  alloc.Destroy();
}