#ifndef _MEMORY_ALLOCATOR_HPP_
#define _MEMORY_ALLOCATOR_HPP_

#include <cstdint>
#include <vector>

#include "./CoalesceAllocator.hpp"
#include "./FixedAllocator.hpp"

namespace memory_manager {
typedef int8_t byte;

const int kFsaCount = 6;
const int k16Bytes = 16;
const int k32Bytes = 32;
const int k64Bytes = 64;
const int k128Bytes = 128;
const int k256Bytes = 256;
const int k512Bytes = 512;
const int k4Megabytes = k4Megabytes;
const int kMinMegabytes = 10 * 1024 * 1024;

class MemoryManager {
 public:
  MemoryManager();
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
  FixedAllocator fixed_allocators_[kFsaCount] = {
      FixedAllocator(k16Bytes, k4Megabytes / k16Bytes),
      FixedAllocator(k32Bytes, k4Megabytes / k32Bytes),
      FixedAllocator(k64Bytes, k4Megabytes / k64Bytes),
      FixedAllocator(k128Bytes, k4Megabytes / k128Bytes),
      FixedAllocator(k256Bytes, k4Megabytes / k256Bytes),
      FixedAllocator(k512Bytes, k4Megabytes / k512Bytes)};
  CoalesceAllocator coalesce_allocator_;
  std::vector<void*> os_allocs_;
};
}  // namespace memory_manager

#endif  // _MEMORY_ALLOCATOR_HPP_
