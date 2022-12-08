#ifdef _DEBUG
#include <cassert>
#include <iostream>
#endif  // _DEBUG

#include "FixedAllocator.hpp"
#include "windows.h"

using memory_manager::FixedAllocator;

FixedAllocator::FixedAllocator(int block_size) {}

FixedAllocator::~FixedAllocator() {}

void FixedAllocator::Init() {}

void FixedAllocator::Destroy() {}

void* FixedAllocator::Alloc(size_t size) { return nullptr; }

void FixedAllocator::Free(void* p) {}

void FixedAllocator::DumpStat() const {}

void FixedAllocator::DumpBlocks() const {}
