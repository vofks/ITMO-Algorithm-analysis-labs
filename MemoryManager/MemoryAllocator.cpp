#include "MemoryAllocator.hpp"

using memory_manager::MemoryAllocator;

MemoryAllocator::MemoryAllocator() {}

MemoryAllocator::~MemoryAllocator() {}

void MemoryAllocator::Init() {}

void MemoryAllocator::Destroy() {}

void* MemoryAllocator::Alloc(size_t size) { return nullptr; }

void MemoryAllocator::Free(void* p) {}

void MemoryAllocator::DumpStat() const {}

void MemoryAllocator::DumpBlocks() const {}
