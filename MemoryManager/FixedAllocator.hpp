#ifndef _FIXED_ALLOCATOR_HPP_
#define _FIXED_ALLOCATOR_HPP_

#include <cstring>

#include "./Allocator.hpp"
#include "./Constants.hpp"

namespace memory_manager {

class FixedAllocator final : public Allocator {
 public:
  explicit FixedAllocator(int blockSize, int pageSize = kMegabyte);
  virtual ~FixedAllocator();

  virtual void Init();
  virtual void Destroy();
  virtual void* Alloc(size_t size);
  virtual bool Free(void* ptr);

#ifdef _DEBUG
  virtual void DumpStat() const;
  virtual void DumpBlocks() const;
#endif  // _DEBUG

 private:
  struct PageHeader {
    PageHeader* next_ = nullptr;
    int freeBlockCount_ = 0;
    int freeListHead_ = 0;
    int blockSize_ = 0;
    int pageSize_ = 0;
    byte* data_ = nullptr;

    void InitFreeList() {
      for (int i = 0; i < freeBlockCount_ - 1; ++i) {
        *(int*)GetBlock(i) = i + 1;
      }

      *(int*)GetBlock(freeBlockCount_ - 1) = kFullPage;

#ifdef _DEBUG
      for (int i = 0; i < freeBlockCount_; ++i) {
        for (int j = 0; j < blockSize_ / (strlen(kDebugFlag) + 1); ++j) {
          byte* ptr = GetDebugBlock(i) + j * (strlen(kDebugFlag) + 1);
          strcpy_s((char*)ptr, strlen(memory_manager::kDebugFlag) + 1,
                   memory_manager::kDebugFlag);
        }
      }

      assert(IsCorrupt() && "Integrity check failed.");
#endif  // _DEBUG
    }

    size_t GetBlockIndex(void* ptr) {
      return ((byte*)ptr - data_) / blockSize_;
    }

    byte* GetBlock(int index) {
#ifdef _DEBUG
      return data_ + blockSize_ + 2 * blockSize_ * index;
#else
      return data_ + blockSize_ * index;
#endif  // _DEBUG
    }

#ifdef _DEBUG
    byte* GetDebugBlock(int index) { return data_ + 2 * blockSize_ * index; }
#endif  // _DEBUG

    byte* GetEmptyBlock() {
      if (freeListHead_ == kFullPage) return nullptr;

      return GetBlock(freeListHead_);
    }

    bool ContainsPtr(void* ptr) {
      return ptr >= data_ && ptr <= (byte*)this + pageSize_;
    }

    bool IsEmpty() {
      return freeBlockCount_ == (pageSize_ - sizeof(PageHeader)) / blockSize_;
    }

    bool IsFull() { return freeListHead_ == kFullPage; }

    bool HasNextPage() { return next_; }

    bool IsValidBlock(void* ptr) {
      return ContainsPtr(ptr) && ((byte*)ptr - data_) % blockSize_ == 0;
    }

#ifdef _DEBUG
    bool IsCorrupt() {
      for (int i = 0; i < freeBlockCount_; ++i) {
        for (int j = 0; j < blockSize_ / (strlen(kDebugFlag) + 1); ++j) {
          byte* ptr = GetDebugBlock(i) + j * (strlen(kDebugFlag) + 1);
          int result = strcmp(kDebugFlag, (char*)ptr);
          if (result != 0) return false;
        }
      }

      return true;
    }
#endif
  };

  int blockSize_;
  int pageSize_;
  void* ptr_;

#ifdef _DEBUG
  bool isInitialized_ = false;
  bool isDestroyed_ = false;
#endif  // _DEBUG

  void* AddPage();
  void FreePage(void* ptr);
  void FreePages();
};
}  // namespace memory_manager

#endif  // _FIXED_ALLOCATOR_HPP_
