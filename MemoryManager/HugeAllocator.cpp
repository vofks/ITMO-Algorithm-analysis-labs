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

#ifdef _DEBUG
  ++pageCount_;
  bytesAllocated_ += size;
#endif  // _DEBUG

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
  PageHeader* previousPage = nullptr;

  while (page) {
    previousPage = page;

    if (!page->ContainsPtr(ptr)) {
      page = page->next_;

      continue;
    }

    previousPage->next_ = page->next_;
    if (page == ptr_) {
      ptr_ = page->next_;
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

  std::cout << "Statistics:" << std::endl;
  std::cout << "\tOS Allocator:\n";
  std::cout << "--------------------" << std::endl;
  std::cout << "Page count: " << pageCount_ << std::endl;
  std::cout << "Allocation size: " << bytesAllocated_ << " bytes" << std::endl;
  std::cout << std::endl;
}

void HugeAllocator::DumpBlocks() const {
  assert(isInitialized_ &&
         "Allocator must be initialized with Init() before being used.");
  assert(!isDestroyed_ && "This allocator has already been destroyed.");

  std::cout << "Dump:" << std::endl;
  std::cout << "\tOS Allocator:\n";
  std::cout << "--------------------" << std::endl;

  PageHeader* page = (PageHeader*)ptr_;
  while (page) {
    std::cout << "Occupied block: " << page->ptr_ << " size: " << page->size_
              << "bytes" << std::endl;

    page = page->next_;
  }

  std::cout << std::endl;
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

void HugeAllocator::FreePage(void* ptr) {
#ifdef _DEBUG
  --pageCount_;
  PageHeader* page = (PageHeader*)ptr;
  bytesAllocated_ -= page->size_;
#endif  // _DEBUG

  VirtualFree(ptr, 0, MEM_RELEASE);
}

void HugeAllocator::FreePages() {
  PageHeader* page = (PageHeader*)ptr_;
  PageHeader* nextPage = nullptr;

#ifdef _DEBUG
  assert(!ptr_,
         "Memory leak. All acquired blocks must be freed before calling "
         "Destroy().");
#endif

  while (page) {
    nextPage = page->next_;

    FreePage(page);

    page = nextPage;
  }
}
