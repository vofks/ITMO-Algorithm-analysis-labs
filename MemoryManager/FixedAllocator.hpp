#ifndef _FIXED_ALLOCATOR_HPP_
#define _FIXED_ALLOCATOR_HPP_

#include "./Constants.hpp"

namespace memory_manager {
const int kFullFreeList = -1;

class FixedAllocator final {
 public:
  FixedAllocator(int blockSize, int pageSize = kMegabyte);
  virtual ~FixedAllocator();

  virtual void Init();
  virtual void Destroy();
  virtual void* Alloc(size_t size);
  virtual void Free(void* p);

#ifdef _DEBUG
  virtual void DumpStat() const;
  virtual void DumpBlocks() const;
#endif  // _DEBUG

 private:
  int blockSize_;
  int pageSize_;
  void* data_;
};
}  // namespace memory_manager

#endif  // _FIXED_ALLOCATOR_HPP_
