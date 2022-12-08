#ifdef _DEBUG
#include <cassert>
#include <iostream>
#endif  // _DEBUG

#include "CoalesceAllocator.hpp"
#include "windows.h"

using memory_manager::CoalesceAllocator;

CoalesceAllocator::CoalesceAllocator() {}

memory_manager::CoalesceAllocator::~CoalesceAllocator() {}

void memory_manager::CoalesceAllocator::Init() {}

void memory_manager::CoalesceAllocator::Destroy() {}

void* CoalesceAllocator::Alloc(size_t size) { return nullptr; }

void CoalesceAllocator::Free(void* p) {}

#ifdef _DEBUG
void memory_manager::CoalesceAllocator::DumpStat() const {}

void memory_manager::CoalesceAllocator::DumpBlocks() const {}
#endif  // _DEBUG
