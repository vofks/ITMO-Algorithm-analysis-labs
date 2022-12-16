#ifdef _DEBUG
#include <cassert>
#include <iostream>
#endif  // _DEBUG

#include "./Constants.hpp"
#include "FixedAllocator.hpp"
#include "windows.h"

using memory_manager::FixedAllocator;

FixedAllocator::FixedAllocator(int blockSize, int pageSize)
    : blockSize_(blockSize), pageSize_(pageSize), ptr_(nullptr) {}

FixedAllocator::~FixedAllocator() { Allocator::~Allocator(); }

void FixedAllocator::Init() {
  Allocator::Init();

  ptr_ = AddPage();
}

void FixedAllocator::Destroy() {
  Allocator::Destroy();

  FreePages();
}

void* FixedAllocator::Alloc(size_t size) {
  Allocator::Alloc(size);

  PageHeader* page = (PageHeader*)ptr_;

#ifdef _DEBUG
  assert(page->IsCorrupt() && "Integrity check failed.");
#endif _DEBUG

  while (page->IsFull()) {
    if (!page->HasNextPage()) {
      page->next_ = (PageHeader*)AddPage();
    }

    page = page->next_;
  }

  byte* emptyBlock = page->GetEmptyBlock();
  int nextFreeBlockIndex = *(int*)emptyBlock;

  --page->freeBlockCount_;
  page->freeListHead_ = nextFreeBlockIndex;

  return (void*)emptyBlock;
}

bool FixedAllocator::Free(void* p) {
  Allocator::Free(p);

  PageHeader* page = (PageHeader*)ptr_;
  PageHeader* previousPage = nullptr;

#ifdef _DEBUG
  assert(page->IsCorrupt() && "Integrity check failed.");
#endif  // _DEBUG

  while (page) {
    if (!page->IsValidBlock(p)) {
      previousPage = page;
      page = page->next_;

      continue;
    }

    *(int*)p = page->freeListHead_;
    page->freeListHead_ = page->GetBlockIndex(p);
    ++page->freeBlockCount_;

    if (page->IsEmpty() && previousPage) {
      previousPage->next_ = page->next_;
      FreePage(page);
    }

    return true;
  }

  return false;
}

#ifdef _DEBUG

// TODO: Implement DumpStat
void FixedAllocator::DumpStat() const {
  assert(isInitialized_ &&
         "Allocator must be initialized with Init() before being used.");
  assert(!isDestroyed_ && "This allocator has already been destroyed.");
}

// TODO: Implement DumpBlocks
void FixedAllocator::DumpBlocks() const {
  assert(isInitialized_ &&
         "Allocator must be initialized with Init() before being used.");
  assert(!isDestroyed_ && "This allocator has already been destroyed.");
}

#endif  //  _DEBUG

void* FixedAllocator::AddPage() {
#ifdef _DEBUG
  void* ptr = VirtualAlloc(nullptr, pageSize_ * 3, MEM_RESERVE | MEM_COMMIT,
                           PAGE_READWRITE);
#else
  void* ptr = VirtualAlloc(nullptr, pageSize_, MEM_RESERVE | MEM_COMMIT,
                           PAGE_READWRITE);
#endif

  PageHeader* page = (PageHeader*)ptr;

  page->freeBlockCount_ = (pageSize_ - sizeof(PageHeader)) / blockSize_;
  page->data_ = (byte*)ptr + sizeof(PageHeader);
  page->blockSize_ = blockSize_;
  page->pageSize_ = pageSize_;

  page->InitFreeList();

  return ptr;
}

void FixedAllocator::FreePage(void* ptr) { VirtualFree(ptr, 0, MEM_RELEASE); }

void FixedAllocator::FreePages() {
  PageHeader* page = (PageHeader*)ptr_;
  PageHeader* nextPage = nullptr;

#ifdef _DEBUG
  assert(!page->HasNextPage() && page->IsEmpty() &&
         "Memory leak. All acquired blocks must be freed before calling "
         "Destroy().");
#endif

  while (page) {
    nextPage = page->next_;

    FreePage((void*)page);

    page = nextPage;
  }
}
