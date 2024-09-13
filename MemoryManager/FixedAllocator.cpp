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

  --freeblockCount_;
  ++occupiedBlockCount_;
#endif  // _DEBUG

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

bool FixedAllocator::Free(void* ptr) {
  Allocator::Free(ptr);

  PageHeader* page = (PageHeader*)ptr_;
  PageHeader* previousPage = nullptr;

#ifdef _DEBUG
  assert(page->IsCorrupt() && "Integrity check failed.");

  ++freeblockCount_;
  --occupiedBlockCount_;
#endif  // _DEBUG

  while (page) {
    if (!page->IsValidBlock(ptr)) {
      previousPage = page;
      page = page->next_;

      continue;
    }

    *(int*)ptr = page->freeListHead_;
    page->freeListHead_ = page->GetBlockIndex(ptr);
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

void FixedAllocator::DumpStat() const {
  assert(isInitialized_ &&
         "Allocator must be initialized with Init() before being used.");
  assert(!isDestroyed_ && "This allocator has already been destroyed.");

  std::cout << "Statistics:" << std::endl;
  std::cout << "\tFSA:\n";
  std::cout << "--------------------" << std::endl;
  std::cout << "Block size: " << blockSize_ << " bytes" << std::endl;
  std::cout << "Page count: " << pageCount_ << std::endl;
  std::cout << "Free block count: " << freeblockCount_ << std::endl;
  std::cout << "Occupied block count: " << occupiedBlockCount_ << std::endl;
  std::cout << "Page size: " << pageSize_ << " bytes" << std::endl;
  std::cout << std::endl;
}

void FixedAllocator::DumpBlocks() const {
  assert(isInitialized_ &&
         "Allocator must be initialized with Init() before being used.");
  assert(!isDestroyed_ && "This allocator has already been destroyed.");

  const size_t blocksPerPage = (pageSize_ - sizeof(PageHeader)) / blockSize_;

  std::cout << "Dump:" << std::endl;
  std::cout << "\tFSA:\n";
  std::cout << "--------------------" << std::endl;
  std::cout << "Block size: " << blockSize_ << " bytes" << std::endl;

  PageHeader* page = (PageHeader*)ptr_;
  size_t pageIndex = 0;

  while (page) {
    std::cout << "Page number: " << pageIndex << std::endl;
    bool isEmptyblock = false;

    for (size_t i = 0; i < blocksPerPage; ++i) {
      auto ptr = page->GetBlock(i);

      int freeBlockIndex = page->freeListHead_;
      while (freeBlockIndex != kFullPage) {
        if (i == freeBlockIndex) {
          std::cout << "Free block: " << (void*)ptr << " Size: " << blockSize_
                    << " bytes" << std::endl;
          isEmptyblock = true;

          break;
        }

        freeBlockIndex = *(int*)page->GetBlock(freeBlockIndex);
      }

      if (isEmptyblock) {
        isEmptyblock = false;

        continue;
      }

      std::cout << "Occupied block: " << (void*)ptr << " Size: " << blockSize_
                << " bytes" << std::endl;
    }

    page = page->next_;
    ++pageIndex;
  }

  std::cout << std::endl;
}

#endif  //  _DEBUG

void* FixedAllocator::AddPage() {
  int size = pageSize_;

#ifdef _DEBUG
  size *= 3;
#endif  // _DEBUG

  void* ptr =
      VirtualAlloc(nullptr, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

  PageHeader* page = new (ptr) PageHeader();

  page->freeBlockCount_ = (pageSize_ - sizeof(PageHeader)) / blockSize_;
  page->data_ = (byte*)ptr + sizeof(PageHeader);
  page->blockSize_ = blockSize_;
  page->pageSize_ = pageSize_;

  page->InitFreeList();

#ifdef _DEBUG
  ++pageCount_;
  freeblockCount_ += page->freeBlockCount_;
#endif

  return ptr;
}

void FixedAllocator::FreePage(void* ptr) {
#ifdef _DEBUG
  PageHeader* page = (PageHeader*)ptr;
  --pageCount_;
  freeblockCount_ -= page->freeBlockCount_;
#endif

  VirtualFree(ptr, 0, MEM_RELEASE);
}

void FixedAllocator::FreePages() {
  PageHeader* page = (PageHeader*)ptr_;
  PageHeader* nextPage = nullptr;

#ifdef _DEBUG
  assert(!page->HasNextPage() && page->IsEmpty() &&
         "Memory leak. All acquired blocks must be freed before calling "
         "Destroy().");
#endif  // _DEBUG

  while (page) {
    nextPage = page->next_;

    FreePage(page);

    page = nextPage;
  }
}
