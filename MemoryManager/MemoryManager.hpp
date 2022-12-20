#ifndef _MEMORY_ALLOCATOR_HPP_
#define _MEMORY_ALLOCATOR_HPP_

#include <vector>

#include "./CoalesceAllocator.hpp"
#include "./Constants.hpp"
#include "./FixedAllocator.hpp"
#include "./HugeAllocator.hpp"

namespace memory_manager {
class MemoryManager {
 public:
  virtual ~MemoryManager();

  virtual void Init();
  virtual void Destroy();
  virtual void* Alloc(size_t size);
  virtual void Free(void* p);

#ifdef _DEBUG
  virtual void DumpStat() const;
  virtual void DumpBlocks() const;
#endif  // _DEBUG

 private:
  FixedAllocator fixedAllocators_[kFSACount] = {
      FixedAllocator(k16Bytes),  FixedAllocator(k32Bytes),
      FixedAllocator(k64Bytes),  FixedAllocator(k128Bytes),
      FixedAllocator(k256Bytes), FixedAllocator(k512Bytes)};
  CoalesceAllocator coalesceAllocator_ = CoalesceAllocator();
  HugeAllocator hugeAllocator_ = HugeAllocator();
};
}  // namespace memory_manager

#endif  // _MEMORY_ALLOCATOR_HPP_
