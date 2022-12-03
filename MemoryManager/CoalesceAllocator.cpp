#include "CoalesceAllocator.hpp"

using memory_manager::CoalesceAllocator;

CoalesceAllocator::CoalesceAllocator() {}

void* CoalesceAllocator::Alloc(size_t size) { return nullptr; }

void CoalesceAllocator::Free(void* p) {}
