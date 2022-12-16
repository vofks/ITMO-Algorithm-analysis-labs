#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "./Constants.hpp"
#include "./FixedAllocator.hpp"
#include "windows.h"

using namespace memory_manager;

int main() {
  FixedAllocator a = FixedAllocator(k16Bytes, kMegabyte);
  a.Init();
  void* ptr = a.Alloc(16 * sizeof(memory_manager::byte));
  void* ptr1 = a.Alloc(16 * sizeof(memory_manager::byte));
  void* ptr2 = a.Alloc(16 * sizeof(memory_manager::byte));

  char* str = (char*)ptr2;
  //*(str - 1) = 'a';
  //*(str + 16) = 'a';

  a.Free(ptr);
  a.Free(ptr1);
  a.Free(ptr2);
  a.Destroy();
}