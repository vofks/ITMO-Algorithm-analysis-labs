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

void FixedAllocator::Destroy() { FreePages(); }

void* FixedAllocator::Alloc(size_t size) {
  PageHeader* header = (PageHeader*)data_;

  while (header->free_list_head_ == kFullFreeList) {
    if (header->next == nullptr) {
      header->next = AllocPage();
    }

    header = header->next;
  }

  int next_free_block =
      *(byte*)header->data + block_size_ * header->free_list_head_;

  void* ptr = (byte*)header->data + block_size_ * header->free_list_head_;
  --header->free_block_count;

  if (header->free_block_count == 0) {
    header->free_list_head_ = kFullFreeList;
  } else if (next_free_block) {
    header->free_list_head_ = next_free_block;
  } else {
    ++header->free_list_head_;
  }

  return ptr;
}

void FixedAllocator::Free(void* p) {}

#ifdef _DEBUG

void FixedAllocator::DumpStat() const {}

void FixedAllocator::DumpBlocks() const {}

#endif  //  _DEBUG

FixedAllocator::PageHeader* FixedAllocator::AllocPage() {
  void* ptr =
      VirtualAlloc(nullptr, block_size_ * num_blocks_ + sizeof(PageHeader),
                   MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

  PageHeader* header = (PageHeader*)ptr;
  header->next = nullptr;
  header->free_list_head_ = 0;
  header->data = (byte*)data_ + sizeof(PageHeader);

  return header;
}

void FixedAllocator::FreePage(PageHeader*& page) {
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
