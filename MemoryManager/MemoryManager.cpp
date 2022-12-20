#ifdef _DEBUG
#include <cassert>
#include <iostream>
#endif  // _DEBUG

#include <algorithm>
#include <vector>

#include "MemoryManager.hpp"
#include "windows.h"

using memory_manager::MemoryManager;

MemoryManager::~MemoryManager() {}

void MemoryManager::Init() {
  for (int i = 0; i < 6; ++i) {
    fixedAllocators_[i].Init();
  }

  coalesceAllocator_.Init();
  hugeAllocator_.Init();
}

void MemoryManager::Destroy() {
  for (int i = 0; i < kFSACount; ++i) {
    fixedAllocators_[i].Destroy();
  }

  coalesceAllocator_.Destroy();
  hugeAllocator_.Destroy();
}

void* MemoryManager::Alloc(size_t size) {
  if (size > kMinMegabytes) {
    return hugeAllocator_.Alloc(size);
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
  for (int i = 0; i < 6; ++i) {
    fixedAllocators_[i].Free(ptr);
  }

  coalesceAllocator_.Free(ptr);
  hugeAllocator_.Free(ptr);
}

#ifdef _DEBUG

void MemoryManager::DumpStat() const {}

void MemoryManager::DumpBlocks() const {}

#endif  // _DEBUG
