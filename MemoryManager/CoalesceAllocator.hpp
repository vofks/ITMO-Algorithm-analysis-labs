#ifndef _COALESCE_ALLOCATOR_HPP_
#define _COALESCE_ALLOCATOR_HPP_

#include "./Allocator.hpp"
#include "./Constants.hpp"

namespace memory_manager {
class CoalesceAllocator final : public Allocator {
 public:
  CoalesceAllocator();
  ~CoalesceAllocator() override;

  void Init() override;
  void Destroy() override;
  void* Alloc(size_t size) override;
  bool Free(void* ptr) override;

#ifdef _DEBUG
  void DumpStat() const override;
  void DumpBlocks() const override;
#endif  // _DEBUG

 private:
  struct BlockHeader final {
    BlockHeader* previous_ = nullptr;
    BlockHeader* next_ = nullptr;
    BlockHeader* previousFreeBlock_ = nullptr;
    BlockHeader* nextFreeBlock_ = nullptr;
    size_t size_ = 0;
    byte* data_ = nullptr;
    bool isFree_ = true;

    bool Fits(int size) { return size <= size_; }
    bool CoversWholeBlock(int size) {
      return size_ == size || size_ - size < sizeof(BlockHeader) + 1;
    }

    bool HasUnusedSpace(int size) {
      return size_ - size > sizeof(BlockHeader) + 1;
    }

    bool HasNext() { return next_; }

    bool IsPreviousFree() { return previous_ && previous_->isFree_; }
    bool IsNextFree() { return next_ && next_->isFree_; }
    bool CanCoalesce() { return IsPreviousFree() || IsNextFree(); }
  };

  struct PageHeader final {
    PageHeader* next_ = nullptr;
    BlockHeader* freeListHead_ = nullptr;
    void* ptr_ = nullptr;

    bool HasNext() { return next_; }

    bool ContainsPtr(void* ptr) {
      return ptr >= ptr_ && ptr <= (byte*)ptr_ + kMegabyte;
    }

    BlockHeader* GetBlock(void* ptr) {
      BlockHeader* block = (BlockHeader*)ptr_;

      while (block) {
        if (block->data_ != ptr) {
          block = block->next_;

          continue;
        }

        return block;
      }

      return nullptr;
    }
  };

  void* AddPage();
  void FreePage(void* ptr);
  void FreePages();
  void AddFreeBlock(BlockHeader* block);
  void RemoveFreeBlock(BlockHeader* block);
  BlockHeader* FindFreeBlock(int size);
  void Coalesce(BlockHeader* block);

  void* ptr_;

#ifdef _DEBUG
  size_t pageCount_ = 0;
  size_t freeBlockCount_ = 0;
  size_t occupiedBlockCount_ = 0;
  size_t pageSize_ = 0;
#endif
};

}  // namespace memory_manager

#endif  // _COALESCE_ALLOCATOR_HPP_
