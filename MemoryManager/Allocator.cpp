#include "./Allocator.hpp"

#include <cassert>

Allocator::~Allocator() {
#ifdef _DEBUG
  assert(
      !isInitialized_ ||
      isDestroyed_ &&
          "Destructor was called on initialized allocator. Allocator must be "
          "destroyed beforehand.");
#endif  // _DEBUG
}

void Allocator::Init() {
#ifdef _DEBUG
  assert(!isInitialized_ &&
         "Attempt to reinitialize. Allocator can be initialized only once.");
  isInitialized_ = true;
#endif  // _DEBUG
}

void Allocator::Destroy() {
#ifdef _DEBUG
  isDestroyed_ = true;
#endif _DEBUG
}

void* Allocator::Alloc(size_t size) {
#ifdef _DEBUG
  assert(isInitialized_ &&
         "Allocator must be initialized with Init() before being used.");
  assert(!isDestroyed_ && "This allocator has already been destroyed.");
#endif  // _DEBUG

  return nullptr;
}

bool Allocator::Free(void* p) {
#ifdef _DEBUG
  assert(isInitialized_ &&
         "Allocator must be initialized with Init() before being used.");
  assert(!isDestroyed_ && "This allocator has already been destroyed.");
#endif  // _DEBUG

  return false;
}
