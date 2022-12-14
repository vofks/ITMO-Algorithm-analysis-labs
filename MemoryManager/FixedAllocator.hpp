#ifndef _FIXED_ALLOCATOR_HPP_
#define _FIXED_ALLOCATOR_HPP_

#include "./Constants.hpp"

namespace memory_manager {

class FixedAllocator final {
 public:
  explicit FixedAllocator(int blockSize, int pageSize = kMegabyte);
  virtual ~FixedAllocator();

  virtual void Init();
  virtual void Destroy();
  virtual void* Alloc(size_t size);
  virtual bool Free(void* p);

#ifdef _DEBUG
  virtual void DumpStat() const;
  virtual void DumpBlocks() const;
#endif  // _DEBUG

 private:
  struct PageHeader {
    PageHeader* next = nullptr;
    int freeBlockCount = 0;
    int freeListHead = 0;
    void* data = nullptr;
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
