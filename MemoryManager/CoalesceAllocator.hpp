#ifndef _COALESCE_ALLOCATOR_HPP_
#define _COALESCE_ALLOCATOR_HPP_

#include "./Allocator.hpp"

namespace memory_manager {
class CoalesceAllocator final : public Allocator {
 public:
  CoalesceAllocator();
  virtual ~CoalesceAllocator();

  virtual void Init();
  virtual void Destroy();
  virtual void* Alloc(size_t size);
  virtual bool Free(void* p);

#ifdef _DEBUG
  virtual void DumpStat() const;
  virtual void DumpBlocks() const;
#endif  // _DEBUG
};
}  // namespace memory_manager

#endif  // _COALESCE_ALLOCATOR_HPP_
