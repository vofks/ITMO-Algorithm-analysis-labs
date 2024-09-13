#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "./MemoryManager.hpp"

using namespace memory_manager;

int main() {
  MemoryManager m = MemoryManager();

  m.Init();

  void* ptr = m.Alloc(15);
  void* ptr1 = m.Alloc(20);
  void* ptr2 = m.Alloc(50);
  void* ptr3 = m.Alloc(120);
  void* ptr4 = m.Alloc(200);
  void* ptr5 = m.Alloc(400);

  //m.DumpStat();
  //m.DumpBlocks();

  m.Free(ptr);
  m.Free(ptr1);
  m.Free(ptr2);
  m.Free(ptr3);
  m.Free(ptr4);
  m.Free(ptr5);

  m.Destroy();
}