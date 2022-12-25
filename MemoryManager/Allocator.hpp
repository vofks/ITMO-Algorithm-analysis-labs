#ifndef _ALLOCATOR_HPP_
#define _ALLOCATOR_HPP_
class Allocator {
 public:
  virtual ~Allocator();

  virtual void Init();
  virtual void Destroy();
  virtual void* Alloc(size_t size);
  virtual bool Free(void* ptr);

#ifdef _DEBUG
  virtual void DumpStat() const = 0;
  virtual void DumpBlocks() const = 0;
#endif  // _DEBUG
 protected:
#ifdef _DEBUG
  bool isInitialized_ = false;
  bool isDestroyed_ = false;
#endif  // _DEBUG
};
#endif  // _ALLOCATOR_HPP_