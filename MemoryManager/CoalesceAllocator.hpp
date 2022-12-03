#ifndef _COALESCE_ALLOCATOR_HPP_
#define _COALESCE_ALLOCATOR_HPP_

namespace memory_manager {
class CoalesceAllocator final {
 public:
  CoalesceAllocator();
  void* Alloc(size_t size);
  void Free(void* p);
};
}  // namespace memory_manager

#endif  // _COALESCE_ALLOCATOR_HPP_
