#ifndef _MEMORY_ALLOCATOR_HPP_
#define _MEMORY_ALLOCATOR_HPP_

#include <cstdint>

#include "./CoalesceAllocator.hpp"
#include "./FixedAllocator.hpp"

namespace memory_manager {
typedef int8_t byte;

const int k16Bytes = 16;
const int k32Bytes = 32;
const int k64Bytes = 64;
const int k128Bytes = 128;
const int k256Bytes = 256;
const int k512Bytes = 512;
const int kMinMegabytes = 10 * 1024 * 1024;

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
  FixedAllocator fixed_allocators_[6] = {
      FixedAllocator(k16Bytes),  FixedAllocator(k32Bytes),
      FixedAllocator(k64Bytes),  FixedAllocator(k128Bytes),
      FixedAllocator(k256Bytes), FixedAllocator(k512Bytes)};
  CoalesceAllocator coalesce_allocator_;
};
}  // namespace memory_manager

#endif  // _MEMORY_ALLOCATOR_HPP_
