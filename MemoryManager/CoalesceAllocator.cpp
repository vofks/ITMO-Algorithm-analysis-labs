#ifdef _DEBUG
#include <cassert>
#include <iostream>
#endif  // _DEBUG

#include "CoalesceAllocator.hpp"
#include "windows.h"

using memory_manager::CoalesceAllocator;

CoalesceAllocator::CoalesceAllocator() : ptr_(nullptr) {}

CoalesceAllocator::~CoalesceAllocator() { Allocator::~Allocator(); }

void CoalesceAllocator::Init() {
  Allocator::Init();
  ptr_ = VirtualAlloc(nullptr, kMegabyte, MEM_RESERVE | MEM_COMMIT,
                      PAGE_READWRITE);
}

void CoalesceAllocator::Destroy() { Allocator::Destroy(); }

void* CoalesceAllocator::Alloc(size_t size) {
  Allocator::Alloc(size);

  return nullptr;
}

bool CoalesceAllocator::Free(void* ptr) {
  Allocator::Free(ptr);
  return false;
}

#ifdef _DEBUG
void CoalesceAllocator::DumpStat() const {}

void CoalesceAllocator::DumpBlocks() const {}
#endif  // _DEBUG
