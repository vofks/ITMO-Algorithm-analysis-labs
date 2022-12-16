#ifndef _COALESCE_ALLOCATOR_HPP_
#define _COALESCE_ALLOCATOR_HPP_

#include "./Allocator.hpp"
#include "./Constants.hpp"

namespace memory_manager {
class CoalesceAllocator final : public Allocator {
 public:
  CoalesceAllocator();
  virtual ~CoalesceAllocator();

  virtual void Init();
  virtual void Destroy();
  virtual void* Alloc(size_t size);
  virtual bool Free(void* ptr);

#ifdef _DEBUG
  virtual void DumpStat() const;
  virtual void DumpBlocks() const;
#endif  // _DEBUG

 private:
  struct BlockHeader final {
    BlockHeader* left_ = nullptr;
    BlockHeader* right_ = nullptr;
    int blockSize_ = 0;
    byte* data_ = nullptr;
  };

  void* ptr_;
};

}  // namespace memory_manager

#endif  // _COALESCE_ALLOCATOR_HPP_
