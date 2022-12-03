#include "FixedAllocator.hpp"

using memory_manager::FixedAllocator;

FixedAllocator::FixedAllocator(int block_size) {}

void* FixedAllocator::Alloc(size_t size) { return nullptr; }

void FixedAllocator::Free(void* p) {}
