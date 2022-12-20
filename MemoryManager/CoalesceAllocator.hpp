#ifndef _COALESCE_ALLOCATOR_HPP_
#define _COALESCE_ALLOCATOR_HPP_

#include "./Allocator.hpp"
#include "./Constants.hpp"

namespace memory_manager {
class CoalesceAllocator final : public Allocator {
 public:
  CoalesceAllocator();
  virtual ~CoalesceAllocator();

  virtual void Init();
  virtual void Destroy();
  virtual void* Alloc(size_t size);
  virtual bool Free(void* ptr);

#ifdef _DEBUG
  virtual void DumpStat() const;
  virtual void DumpBlocks() const;
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
      return ptr > ptr_ && ptr < (byte*)ptr_ + kMegabyte;
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

  void* ptr_;

  void* AddPage();
  void FreePage(void* ptr);
  void FreePages();
  void* SplitBlock(BlockHeader* block, int size);
  void AddFreeBlock(BlockHeader* block);
  void RemoveFreeBlock(BlockHeader* block);
  BlockHeader* FindFreeBlock(int size);
  void Coalesce(BlockHeader* block);
};

}  // namespace memory_manager

#endif  // _COALESCE_ALLOCATOR_HPP_
