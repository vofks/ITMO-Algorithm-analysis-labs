#include "MemoryManager.hpp"

using memory_manager::MemoryAllocator;

MemoryAllocator::MemoryAllocator() {}

MemoryAllocator::~MemoryAllocator() {}

void MemoryAllocator::Init() {}

void MemoryAllocator::Destroy() {}

void* MemoryAllocator::Alloc(size_t size) {
  if (size > kMinMegabytes) {
    // TODO: Ask the system
  }
  if (size > k512Bytes) {
    return coalesce_allocator_.Alloc(size);
  }
  if (size > k256Bytes) {
    return fixed_allocators_[5].Alloc(size);
  }
  if (size > k128Bytes) {
    return fixed_allocators_[4].Alloc(size);
  }
  if (size > k64Bytes) {
    return fixed_allocators_[3].Alloc(size);
  }
  if (size > k32Bytes) {
    return fixed_allocators_[2].Alloc(size);
  }
  if (size > k16Bytes) {
    return fixed_allocators_[1].Alloc(size);
  }

  return fixed_allocators_[0].Alloc(size);
}

void MemoryAllocator::Free(void* p) {}

void MemoryAllocator::DumpStat() const {}

void MemoryAllocator::DumpBlocks() const {}
