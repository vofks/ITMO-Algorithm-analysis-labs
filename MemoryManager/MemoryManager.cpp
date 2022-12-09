#ifdef _DEBUG
#include <cassert>
#include <iostream>
#endif  // _DEBUG

#include <algorithm>
#include <vector>

#include "MemoryManager.hpp"
#include "windows.h"

using memory_manager::MemoryManager;

MemoryManager::MemoryManager() {}

MemoryManager::~MemoryManager() {}

void MemoryManager::Init() {
  for (int i = 0; i < 6; ++i) {
    fixed_allocators_[i].Init();
  }

  coalesce_allocator_.Init();
}

void MemoryManager::Destroy() {
  for (int i = 0; i < kFsaCount; ++i) {
    fixed_allocators_[i].Destroy();
  }

  coalesce_allocator_.Destroy();

  for (auto p : os_allocs_) {
    VirtualFree(p, 0, MEM_RELEASE);
  }
}

void* MemoryManager::Alloc(size_t size) {
  if (size > kMinMegabytes) {
    void* ptr =
        VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    os_allocs_.push_back(ptr);

    return ptr;
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

void MemoryManager::Free(void* ptr) {
  bool success = VirtualFree(ptr, 0, MEM_RELEASE);

  if (success) {
    os_allocs_.erase(std::find(os_allocs_.begin(), os_allocs_.end(), ptr));

    return;
  }

  // TODO: установить принадлежность указателя аллокатору
}

#ifdef _DEBUG

void MemoryManager::DumpStat() const {}

void MemoryManager::DumpBlocks() const {}

#endif  // _DEBUG
