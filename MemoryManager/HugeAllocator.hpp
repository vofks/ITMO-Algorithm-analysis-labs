#ifndef _HUGE_ALLOCATOR_HPP_
#define _HUGE_ALLOCATOR_HPP_

#include "./Allocator.hpp"
#include "./Constants.hpp"

namespace memory_manager {
class HugeAllocator final : public Allocator {
 public:
  HugeAllocator();
  ~HugeAllocator() override;

  void Init() override;
  void Destroy() override;
  virtual void* Alloc(size_t size) override;
  virtual bool Free(void* ptr) override;

#ifdef _DEBUG
  virtual void DumpStat() const override;
  virtual void DumpBlocks() const override;
#endif  // _DEBUG

 private:
  struct PageHeader final {
    PageHeader* next_ = nullptr;
    size_t size_ = 0;
    void* ptr_ = nullptr;

    bool ContainsPtr(void* ptr) {
      return ptr >= ptr_ && ptr <= (byte*)ptr_ + size_;
    }
  };

  void* AddPage(size_t size);
  void FreePage(void* ptr);
  void FreePages();

  void* ptr_;
};
}  // namespace memory_manager

#endif  // _HUGE_ALLOCATOR_HPP_