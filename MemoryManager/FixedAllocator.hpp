#ifndef _FIXED_ALLOCATOR_HPP_
#define _FIXED_ALLOCATOR_HPP_

namespace memory_manager {
class FixedAllocator final {
 public:
  FixedAllocator(int block_size);

  void* Alloc(size_t size);
  void Free(void* p);
 private:
};
}  // namespace memory_manager

#endif  // _FIXED_ALLOCATOR_HPP_
