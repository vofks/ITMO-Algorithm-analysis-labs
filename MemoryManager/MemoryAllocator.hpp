#ifndef _MEMORY_ALLOCATOR_HPP_
#define _MEMORY_ALLOCATOR_HPP_

namespace memory_manager {
class MemoryAllocator {
 public:
  MemoryAllocator();
  virtual ~MemoryAllocator();
  virtual void Init();
  virtual void Destroy();
  virtual void* Alloc(size_t size);
  virtual void Free(void* p);
  virtual void DumpStat() const;
  virtual void DumpBlocks() const;

 private:
};
}  // namespace memory_manager

#endif  // _MEMORY_ALLOCATOR_HPP_
