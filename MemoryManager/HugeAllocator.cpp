#ifdef _DEBUG
#include <cassert>
#include <iostream>
#endif  // _DEBUG

#include "HugeAllocator.hpp"
#include "windows.h"

using memory_manager::HugeAllocator;

HugeAllocator::HugeAllocator() : ptr_(nullptr) {}

HugeAllocator::~HugeAllocator() { Allocator::~Allocator(); }

void HugeAllocator::Init() { Allocator::Init(); }

void HugeAllocator::Destroy() {
  Allocator::Destroy();

  FreePages();
}

void* HugeAllocator::Alloc(size_t size) {
  Allocator::Alloc(size);

  if (!ptr_) {
    ptr_ = AddPage(size);
    PageHeader* page = (PageHeader*)ptr_;

    return page->ptr_;
  }

  PageHeader* page = (PageHeader*)ptr_;

  while (page->next_) {
    page = page->next_;
  }

  page->next_ = (PageHeader*)AddPage(size);

  return page->next_->ptr_;
}

bool HugeAllocator::Free(void* ptr) {
  Allocator::Free(ptr);

  PageHeader* page = (PageHeader*)ptr_;

  while (page) {
    if (!page->ContainsPtr(ptr)) {
      page = page->next_;

      continue;
    }

    FreePage(page);

    return true;
  }

  return false;
}

#ifdef _DEBUG
void HugeAllocator::DumpStat() const {
  assert(isInitialized_ &&
         "Allocator must be initialized with Init() before being used.");
  assert(!isDestroyed_ && "This allocator has already been destroyed.");
}

void HugeAllocator::DumpBlocks() const {
  assert(isInitialized_ &&
         "Allocator must be initialized with Init() before being used.");
  assert(!isDestroyed_ && "This allocator has already been destroyed.");
}

#endif  // _DEBUG

void* HugeAllocator::AddPage(size_t size) {
  size_t allocationSize = size;
#ifdef _DEBUG
  allocationSize *= 2;
#endif  // _DEBUG

  void* ptr = VirtualAlloc(nullptr, allocationSize, MEM_RESERVE | MEM_COMMIT,
                           PAGE_READWRITE);

  PageHeader* page = new (ptr) PageHeader();
  page->size_ = size;
  page->ptr_ = (byte*)ptr + sizeof(PageHeader);

  return ptr;
}

void HugeAllocator::FreePage(void* ptr) { VirtualFree(ptr, 0, MEM_RELEASE); }

void HugeAllocator::FreePages() {
  PageHeader* page = (PageHeader*)ptr_;
  PageHeader* nextPage = nullptr;

  // TODO: add leak check

  while (page) {
    nextPage = page->next_;

    FreePage(page);

    page = nextPage;
  }
}
