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

FixedAllocator::~FixedAllocator() {
#ifdef _DEBUG
  assert(
      !isInitialized_ ||
      isDestroyed_ &&
          "Destructor was called on initialized allocator. Allocator must be "
          "destroyed beforehand.");
#endif  // _DEBUG
}

void FixedAllocator::Init() {
#ifdef _DEBUG
  assert(!isInitialized_ &&
         "Attempt to reinitialize. Allocator can be initialized only once.");
  isInitialized_ = true;
#endif  // _DEBUG

  ptr_ = AddPage();
}

void FixedAllocator::Destroy() {
#ifdef _DEBUG
  isDestroyed_ = true;
#endif _DEBUG

  FreePages();
}

void* FixedAllocator::Alloc(size_t size) {
#ifdef _DEBUG
  assert(size <= blockSize_ &&
         "Allocator is too small. Consider using a wider block.");
  assert(isInitialized_ &&
         "Allocator must be initialized with Init() before being used.");
  assert(!isDestroyed_ && "This allocator has already been destroyed.");
#endif  // _DEBUG

  PageHeader* header = (PageHeader*)ptr_;

  while (header->freeListHead == kFullPage) {
    if (header->next == nullptr) {
      header->next = (PageHeader*)AddPage();
    }

    header = header->next;
  }

  int freeBlockIndex = header->freeListHead;
  byte* freeBlock = (byte*)header->data + freeBlockIndex * blockSize_;
  int nextFreeBlockIndex = *(int*)freeBlock;

  --header->freeBlockCount;
  header->freeListHead = nextFreeBlockIndex;

  return (void*)freeBlock;
}

bool FixedAllocator::Free(void* p) {
#ifdef _DEBUG
  assert(isInitialized_ &&
         "Allocator must be initialized with Init() before being used.");
  assert(!isDestroyed_ && "This allocator has already been destroyed.");
#endif  // _DEBUG

  PageHeader* header = (PageHeader*)ptr_;
  PageHeader* previousHeader = nullptr;

  while (header) {
    if (p >= header->data && p <= (byte*)header + pageSize_) {
      if (((byte*)p - (byte*)header->data) % blockSize_ != 0) {
        return false;
      }

      *(int*)p = header->freeListHead;
      header->freeListHead = ((byte*)p - (byte*)header->data) / blockSize_;
      ++header->freeBlockCount;

      // Delete empty page
      if (header->freeBlockCount ==
              (pageSize_ - sizeof(PageHeader)) / blockSize_ &&
          previousHeader) {
        previousHeader->next = header->next;
        FreePage(header);
      }

      return true;
    }

    previousHeader = header;
    header = header->next;
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
  void* ptr = VirtualAlloc(nullptr, pageSize_, MEM_RESERVE | MEM_COMMIT,
                           PAGE_READWRITE);

  PageHeader* header = (PageHeader*)ptr;
  header->freeBlockCount = (pageSize_ - sizeof(PageHeader)) / blockSize_;
  header->data = (byte*)ptr + sizeof(PageHeader);

  // TODO: add debug flags

  for (int i = 0; i < header->freeBlockCount - 1; ++i) {
    *(int*)((byte*)header->data + i * blockSize_) = i + 1;
  }

  // TODO: refactor && cleanup
  *(int*)((byte*)header->data + (header->freeBlockCount - 1) * blockSize_) =
      kFullPage;

  return ptr;
}

void FixedAllocator::FreePage(void* ptr) { VirtualFree(ptr, 0, MEM_RELEASE); }

void memory_manager::FixedAllocator::FreePages() {
  PageHeader* header = (PageHeader*)ptr_;
  PageHeader* nextHeader = nullptr;

#ifdef _DEBUG
  bool isEmptyPage =
      header->freeBlockCount == (pageSize_ - sizeof(PageHeader)) / blockSize_;
  assert(!header->next && isEmptyPage &&
         "Memory leak. All acquired blocks must be freed before calling "
         "Destroy().");
#endif

  while (header) {
    nextHeader = header->next;

    FreePage((void*)header);

    header = nextHeader;
  }
}
