#ifdef _DEBUG
#include <cassert>
#include <iostream>
#endif  // _DEBUG

#include "FixedAllocator.hpp"
#include "windows.h"

using memory_manager::FixedAllocator;

FixedAllocator::FixedAllocator(int block_size, int num_blocks)
    : block_size_(block_size), num_blocks_(num_blocks), data_(nullptr) {}

FixedAllocator::~FixedAllocator() {}

void FixedAllocator::Init() { data_ = AllocPage(); }

void FixedAllocator::Destroy() {
  PageHeader* header = (PageHeader*)data_;
  FreePages(header);
}

void* FixedAllocator::Alloc(size_t size) { return nullptr; }

void FixedAllocator::Free(void* p) {}

#ifdef _DEBUG

void FixedAllocator::DumpStat() const {}

void FixedAllocator::DumpBlocks() const {}

#endif  //  _DEBUG

void* memory_manager::FixedAllocator::AllocPage() {
  void* ptr =
      VirtualAlloc(nullptr, block_size_ * num_blocks_ + sizeof(PageHeader),
                   MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

  PageHeader* header = (PageHeader*)ptr;
  header->next = nullptr;
  header->free_block_count = num_blocks_;
  header->data = (byte*)data_ + sizeof(PageHeader);

  return ptr;
}

void memory_manager::FixedAllocator::FreePage(PageHeader*& page) {
#ifdef DEBUG
  assert(page->next != nullptr);
  assert(VirtualFree((void*)page, 0, MEM_RELEASE));
#endif  // DEBUG

  VirtualFree((void*)page, 0, MEM_RELEASE);
}

void memory_manager::FixedAllocator::FreePages() {
  PageHeader* page = (PageHeader*)data_;
  PageHeader* next = nullptr;

  while (page != nullptr) {
    next = page->next;

    FreePage(page);

    page = next;
  }
}
