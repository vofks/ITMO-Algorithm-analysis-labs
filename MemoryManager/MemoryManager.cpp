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
    fixedAllocators_[i].Init();
  }

  coalesceAllocator_.Init();
}

void MemoryManager::Destroy() {
  for (int i = 0; i < kFSACount; ++i) {
    fixedAllocators_[i].Destroy();
  }

  coalesceAllocator_.Destroy();

  for (auto p : osAllocs_) {
    VirtualFree(p, 0, MEM_RELEASE);
  }
}

void* MemoryManager::Alloc(size_t size) {
  if (size > kMinMegabytes) {
    void* ptr =
        VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    osAllocs_.push_back(ptr);

    return ptr;
  }

  if (size > k512Bytes) {
    return coalesceAllocator_.Alloc(size);
  }

  if (size > k256Bytes) {
    return fixedAllocators_[5].Alloc(size);
  }

  if (size > k128Bytes) {
    return fixedAllocators_[4].Alloc(size);
  }

  if (size > k64Bytes) {
    return fixedAllocators_[3].Alloc(size);
  }

  if (size > k32Bytes) {
    return fixedAllocators_[2].Alloc(size);
  }

  if (size > k16Bytes) {
    return fixedAllocators_[1].Alloc(size);
  }

  return fixedAllocators_[0].Alloc(size);
}

void MemoryManager::Free(void* ptr) {
  bool success = VirtualFree(ptr, 0, MEM_RELEASE);

  if (success) {
    osAllocs_.erase(std::find(osAllocs_.begin(), osAllocs_.end(), ptr));

    return;
  }

  // TODO: установить принадлежность указателя аллокатору
}

#ifdef _DEBUG

void MemoryManager::DumpStat() const {}

void MemoryManager::DumpBlocks() const {}

#endif  // _DEBUG
