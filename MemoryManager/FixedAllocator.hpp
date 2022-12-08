#ifndef _FIXED_ALLOCATOR_HPP_
#define _FIXED_ALLOCATOR_HPP_

namespace memory_manager {
class FixedAllocator final {
 public:
  FixedAllocator(int block_size);
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
};
}  // namespace memory_manager

#endif  // _FIXED_ALLOCATOR_HPP_
