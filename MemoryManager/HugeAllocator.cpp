#ifdef _DEBUG
#include <cassert>
#endif

#include "HugeAllocator.hpp"

using memory_manager::HugeAllocator;

HugeAllocator::HugeAllocator() : ptr_(nullptr) {}

HugeAllocator::~HugeAllocator() { Allocator::~Allocator(); }

void HugeAllocator::Init() { Allocator::Init(); }

void HugeAllocator::Destroy() { Allocator::Destroy(); }

void* HugeAllocator::Alloc(size_t size) {
  Allocator::Alloc(size);

  return nullptr;
}

bool HugeAllocator::Free(void* ptr) {
  Allocator::Free(ptr);

  return false;
}

#ifdef _DEBUG
void HugeAllocator::DumpStat() const {
  assert(isInitialized_ &&
         "Allocator must be initialized with Init() before being used.");
  assert(!isDestroyed_ && "This allocator has already been destroyed.");
}

void HugeAllocator::DumpBlocks() const {
  assert(isInitialized_ &&
         "Allocator must be initialized with Init() before being used.");
  assert(!isDestroyed_ && "This allocator has already been destroyed.");
}
#endif  // _DEBUG
