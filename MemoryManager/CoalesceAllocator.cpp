#ifdef _DEBUG
#include <cassert>
#include <iostream>
#endif  // _DEBUG

#include "CoalesceAllocator.hpp"
#include "windows.h"

using memory_manager::CoalesceAllocator;

CoalesceAllocator::CoalesceAllocator() : ptr_(nullptr) {}

CoalesceAllocator::~CoalesceAllocator() { Allocator::~Allocator(); }

void CoalesceAllocator::Init() {
  Allocator::Init();

  ptr_ = AddPage();
}

void CoalesceAllocator::Destroy() {
  Allocator::Destroy();

  FreePages();
}

void* CoalesceAllocator::Alloc(size_t size) {
  Allocator::Alloc(size);

  PageHeader* page = (PageHeader*)ptr_;
  BlockHeader* block = FindFreeBlock(size);
  block->isFree_ = false;

  RemoveFreeBlock(block);

  if (!block->HasUnusedSpace(size)) {
    return block->data_;
  }

  // Shrink unused space
  block->size_ = block->size_ - size;

  byte* newBlockAddress = block->data_ + block->size_;
  BlockHeader* newBlock = new (newBlockAddress) BlockHeader();

  newBlock->next_ = block->next_;
  newBlock->previous_ = block;

  AddFreeBlock(newBlock);

  return block->data_;
}

bool CoalesceAllocator::Free(void* ptr) {
  Allocator::Free(ptr);

  PageHeader* page = (PageHeader*)ptr_;

  while (page) {
    if (!page->ContainsPtr(ptr)) {
      page = page->next_;

      continue;
    }

    BlockHeader* block = page->GetBlock(ptr);
    block->isFree_ = true;

    if (!block->CanCoalesce()) {
      AddFreeBlock(block);

      return true;
    }

    Coalesce(block);

    return true;
  }

  return false;
}

#ifdef _DEBUG
void CoalesceAllocator::DumpStat() const {}

void CoalesceAllocator::DumpBlocks() const {}
#endif  // _DEBUG

void* CoalesceAllocator::AddPage() {
  void* ptr = VirtualAlloc(nullptr, kMegabyte, MEM_RESERVE | MEM_COMMIT,
                           PAGE_READWRITE);
  PageHeader* page = (PageHeader*)ptr;
  page->ptr_ = (byte*)page + sizeof(PageHeader);

  BlockHeader* block = (BlockHeader*)page->ptr_;
  block->data_ = (byte*)block + sizeof(BlockHeader);
  block->size_ = kMegabyte - sizeof(PageHeader) - sizeof(BlockHeader);

  page->freeListHead_ = block;

  return ptr;
}

void CoalesceAllocator::FreePage(void* ptr) {
  VirtualFree(ptr, 0, MEM_RELEASE);
}

void CoalesceAllocator::FreePages() {
  PageHeader* page = (PageHeader*)ptr_;
  PageHeader* nextPage = nullptr;

  // TODO: add leak check

  while (page->next_) {
    nextPage = page->next_;

    FreePage(page);

    page = nextPage;
  }
}

void* CoalesceAllocator::SplitBlock(BlockHeader* block, int payloadSize) {
  int unusedSpace = block->size_ - payloadSize;
  block->size_ = payloadSize;
  block->isFree_ = false;

  BlockHeader* newBlock = new (block->data_ + payloadSize) BlockHeader();
  newBlock->size_ = unusedSpace;
  newBlock->data_ = (byte*)newBlock + sizeof(BlockHeader);
  newBlock->previous_ = block;
  newBlock->next_ = block->next_;
  if (newBlock->next_) {
    newBlock->next_->previous_ = newBlock;
  }

  block->next_ = newBlock;

  AddFreeBlock(newBlock);

  return block->data_;
}

void CoalesceAllocator::AddFreeBlock(BlockHeader* block) {
  PageHeader* page = (PageHeader*)ptr_;

  block->nextFreeBlock_ = page->freeListHead_;
  page->freeListHead_->previousFreeBlock_ = block;
  page->freeListHead_ = block;
}

void CoalesceAllocator::RemoveFreeBlock(BlockHeader* block) {
  PageHeader* page = (PageHeader*)ptr_;

  if (block == page->freeListHead_) {
    page->freeListHead_ = block->nextFreeBlock_;

    if (block->nextFreeBlock_) {
      block->nextFreeBlock_->previousFreeBlock_ = nullptr;
    }

    return;
  }

  if (block->previousFreeBlock_) {
    block->previousFreeBlock_->nextFreeBlock_ = block->nextFreeBlock_;
  }

  if (block->nextFreeBlock_) {
    block->nextFreeBlock_->previousFreeBlock_ = block->previousFreeBlock_;
  }
}

// Linear search, first fit strategy
CoalesceAllocator::BlockHeader* CoalesceAllocator::FindFreeBlock(int size) {
  PageHeader* currentPage = (PageHeader*)ptr_;
  BlockHeader* currentBlock = currentPage->freeListHead_;

  while (!currentBlock->Fits(size)) {
    currentBlock = currentBlock->nextFreeBlock_;

    if (currentBlock) {
      continue;
    }

    if (!currentPage->HasNext()) {
      currentPage->next_ = (PageHeader*)AddPage();
    }

    currentPage = currentPage->next_;
    currentBlock = currentPage->freeListHead_;
  }

  return currentBlock;
}

void memory_manager::CoalesceAllocator::Coalesce(BlockHeader* block) {
  // Left neighbor coalesce
  if (block->IsPreviousFree()) {
    BlockHeader* previousBlock = block->previous_;
    previousBlock->next_ = block->next_;
    previousBlock->size_ += block->size_ + sizeof(BlockHeader);

    if (block->HasNext()) {
      block->next_->previous_ = previousBlock;
    }

    // Transform for right neighbor coalesce test
    block = previousBlock;
  }

  // Right neighbor coalesce
  if (block->IsNextFree()) {
    BlockHeader* nextBlock = block->next_;
    block->next_ = nextBlock->next_;
    block->size_ += nextBlock->size_ + sizeof(BlockHeader);

    if (nextBlock->HasNext()) {
      nextBlock->next_->previous_ = block;
    }

    // Rewire free list
    block->previousFreeBlock_ = nextBlock->previousFreeBlock_;
    block->nextFreeBlock_ = nextBlock->nextFreeBlock_;

    if (block->previousFreeBlock_) {
      block->previousFreeBlock_->nextFreeBlock_ = block;
    }

    if (block->nextFreeBlock_) {
      block->nextFreeBlock_->previousFreeBlock_ = block;
    }
  }
}
