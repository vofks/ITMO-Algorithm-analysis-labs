#ifndef _COALESCE_ALLOCATOR_HPP_
#define _COALESCE_ALLOCATOR_HPP_

namespace memory_manager {
class CoalesceAllocator final {
 public:
  CoalesceAllocator();
  virtual ~CoalesceAllocator();

  virtual void Init();
  virtual void Destroy();
  virtual void* Alloc(size_t size);
  virtual void Free(void* p);

#ifdef _DEBUG
  virtual void DumpStat() const;
  virtual void DumpBlocks() const;
#endif  // _DEBUG
};
}  // namespace memory_manager

#endif  // _COALESCE_ALLOCATOR_HPP_
