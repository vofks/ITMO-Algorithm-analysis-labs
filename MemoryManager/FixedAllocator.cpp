#ifdef _DEBUG
#include <cassert>
#include <iostream>
#endif  // _DEBUG

#include "FixedAllocator.hpp"
#include "windows.h"

using memory_manager::FixedAllocator;

FixedAllocator::FixedAllocator(int blockSize, int pageSize)
    : blockSize_(blockSize), pageSize_(pageSize), data_(nullptr) {}

FixedAllocator::~FixedAllocator() {}

void FixedAllocator::Init() {  }

void FixedAllocator::Destroy() {  }

void* FixedAllocator::Alloc(size_t size) { return nullptr; }

void FixedAllocator::Free(void* p) {}

#ifdef _DEBUG

void FixedAllocator::DumpStat() const {}

void FixedAllocator::DumpBlocks() const {}

#endif  //  _DEBUG
