#ifndef ARRAYLIST_HPP_
#define ARRAYLIST_HPP_

#include <algorithm>

const int kDefaultCapacity = 16;
const int kCapacityFactor = 2;

template <typename T>
class ArrayList final {
 public:
  class Iterator;
  class ConstIterator;

  ArrayList();
  ArrayList(int);
  ArrayList(const ArrayList<T>&);
  ArrayList(ArrayList<T>&&) noexcept;

  const T& operator[](int) const;
  T& operator[](int);
  ArrayList<T>& operator=(ArrayList<T>);
  /*ArrayList<T>& operator=(ArrayList<T>&&);*/

  ~ArrayList();

  int Insert(const T&);
  int Insert(int, const T&);
  void Remove(int);
  int Size() const;
  int Capacity() const;

  Iterator iterator();
  ConstIterator iterator() const;
  Iterator reverseIterator();
  ConstIterator reverseIterator() const;
  Iterator begin();
  ConstIterator cbegin() const;
  Iterator end();
  ConstIterator cend() const;

 private:
  void Swap(ArrayList&);

  int size_;
  int capacity_;
  T* ptr_;
};

template <typename T>
class ArrayList<T>::Iterator {
 public:
  Iterator(ArrayList<T>&, int, int);

  Iterator operator++();
  Iterator operator++(int);
  Iterator operator--();
  Iterator operator--(int);
  bool operator==(const Iterator&);
  bool operator!=(const Iterator&);
  T& operator*();

  T& Get() const;
  void Set(const T&);
  void Next();
  bool HasCurrent() const;

 private:
  int index_;
  int offset_;
  ArrayList<T>& array_;
};

template <typename T>
class ArrayList<T>::ConstIterator {
 public:
  ConstIterator(const ArrayList<T>&, int, int);

  ConstIterator operator++();
  ConstIterator operator++(int);
  ConstIterator operator--();
  ConstIterator operator--(int);
  bool operator==(const ConstIterator&);
  bool operator!=(const ConstIterator&);
  const T& operator*();

  const T& Get() const;
  void Next();
  bool HasCurrent() const;

 private:
  int index_;
  int offset_;
  const ArrayList<T>& array_;
};

template <typename T>
inline ArrayList<T>::ArrayList() : size_(0), capacity_(kDefaultCapacity) {
  ptr_ = (T*)malloc(capacity_ * sizeof(T));
}

template <typename T>
inline ArrayList<T>::ArrayList(int capacity) : size_(0) {
  if (capacity <= 0) {
    capacity = kDefaultCapacity;
  }

  capacity_ = capacity;
  ptr_ = (T*)malloc(capacity_ * sizeof(T));
}

template <typename T>
inline ArrayList<T>::ArrayList(const ArrayList<T>& other) {
  size_ = other.size_;
  capacity_ = other.capacity_;
  ptr_ = (T*)malloc(capacity_ * sizeof(T));

  for (int i = 0; i < size_; ++i) {
    new (ptr_ + i) T(other.ptr_[i]);
  }
}

template <typename T>
inline ArrayList<T>::ArrayList(ArrayList<T>&& rhs) noexcept {
  capacity_ = rhs.capacity_;
  size_ = rhs.size_;
  ptr_ = rhs.ptr_;

  rhs.ptr_ = nullptr;
  rhs.capacity_ = 0;
  rhs.size_ = 0;
}

template <typename T>
inline const T& ArrayList<T>::operator[](int i) const {
  return ptr_[i];
}

template <typename T>
inline T& ArrayList<T>::operator[](int i) {
  return ptr_[i];
}

template <typename T>
inline ArrayList<T>& ArrayList<T>::operator=(ArrayList<T> other) {
  Swap(other);

  return *this;
}

//template <typename T>
//inline ArrayList<T>& ArrayList<T>::operator=(ArrayList<T>&& rhs) {
//  for (int i = 0; i < size_; ++i) {
//    ptr_[i].~T();
//  }
//
//  free(ptr_);
//
//  capacity_ = rhs.capacity_;
//  size_ = rhs.size_;
//  ptr_ = rhs.ptr_;
//
//  rhs.ptr_ = nullptr;
//  rhs.capacity_ = 0;
//  rhs.size_ = 0;
//}

template <typename T>
inline ArrayList<T>::~ArrayList() {
  for (int i = 0; i < size_; ++i) {
    ptr_[i].~T();
  }

  free(ptr_);
}

template <typename T>
inline int ArrayList<T>::Insert(const T& value) {
  if (size_ + 1 > capacity_) {
    capacity_ *= kCapacityFactor;

    T* ptr_temp = (T*)malloc(capacity_ * sizeof(T));

    for (int i = 0; i < size_; ++i) {
      new (ptr_temp + i) T(std::move(ptr_[i]));
      ptr_[i].~T();
    }

    free(ptr_);
    ptr_ = ptr_temp;
  }

  new (ptr_ + size_) T(std::move(value));

  size_++;

  return size_ - 1;
}

template <typename T>
inline int ArrayList<T>::Insert(int index, const T& value) {
  if (index < 0 || index > size_) {
    return -1;
  }

  if (index == size_) {
    Insert(value);
  }

  if (size_ + 1 > capacity_) {
    capacity_ *= kCapacityFactor;
    T* ptr_temp = (T*)malloc(capacity_ * sizeof(T));

    for (int i = 0; i < index; ++i) {
      new (ptr_temp + i) T(std::move(ptr_[i]));
      ptr_[i].~T();
    }

    new (ptr_temp + index) T(std::move(value));

    for (int i = size_ - 1; i > index; --i) {
      new (ptr_temp + i + 1) T(std::move(ptr_[i]));
      ptr_[i].~T();
    }

    free(ptr_);
    ptr_ = ptr_temp;
  } else {
    new (ptr_ + size_) T(std::move(ptr_[size_ - 1]));

    for (int i = size_ - 2; i > index; --i) {
      ptr_[i + 1] = std::move(ptr_[i]);
    }

    ptr_[index] = std::move(value);
  }

  size_++;

  return index;
}

template <typename T>
inline void ArrayList<T>::Remove(int index) {
  if (index < 0 || index > size_ - 1) {
    return;
  }

  for (int i = index + 1; i < size_; ++i) {
    ptr_[i - 1] = std::move(ptr_[i]);
  }

  ptr_[size_ - 1].~T();
  --size_;
}

template <typename T>
inline int ArrayList<T>::Size() const {
  return size_;
}

template <typename T>
inline int ArrayList<T>::Capacity() const {
  return capacity_;
}

template <typename T>
inline typename ArrayList<T>::Iterator ArrayList<T>::iterator() {
  return ArrayList<T>::Iterator(*this, 0, 1);
}

template <typename T>
inline typename ArrayList<T>::ConstIterator ArrayList<T>::iterator() const {
  return ArrayList<T>::ConstIterator(*this, 0, 1);
}

template <typename T>
inline typename ArrayList<T>::Iterator ArrayList<T>::reverseIterator() {
  return ArrayList<T>::Iterator(*this, this->size_ - 1, -1);
}

template <typename T>
inline typename ArrayList<T>::ConstIterator ArrayList<T>::reverseIterator()
    const {
  return ArrayList<T>::ConstIterator(*this, this->size_ - 1, -1);
}

template <typename T>
inline typename ArrayList<T>::Iterator ArrayList<T>::begin() {
  return iterator();
}

template <typename T>
inline typename ArrayList<T>::Iterator ArrayList<T>::end() {
  return ArrayList<T>::Iterator(*this, size_, 1);
}

template <typename T>
inline typename ArrayList<T>::ConstIterator ArrayList<T>::cbegin() const {
  return iterator();
}

template <typename T>
inline typename ArrayList<T>::ConstIterator ArrayList<T>::cend() const {
  return ArrayList<T>::ConstIterator(*this, size_, 1);
}

template <typename T>
inline void ArrayList<T>::Swap(ArrayList<T>& rhs) {
  std::swap(size_, rhs.size_);
  std::swap(capacity_, rhs.capacity_);
  std::swap(ptr_, rhs.ptr_);
}

template <typename T>
inline ArrayList<T>::Iterator::Iterator(ArrayList<T>& array, int index,
                                        int offset)
    : array_(array) {
  index_ = index;
  offset_ = offset;
}

template <typename T>
inline T& ArrayList<T>::Iterator::Get() const {
  return array_[index_];
}

template <typename T>
inline void ArrayList<T>::Iterator::Set(const T& value) {
  array_[index_] = value;
}

template <typename T>
inline void ArrayList<T>::Iterator::Next() {
  index_ += offset_;
}

template <typename T>
inline bool ArrayList<T>::Iterator::HasCurrent() const {
  return (index_ >= 0) && (index_ <= array_.size_ - 1);
}

template <typename T>
inline typename ArrayList<T>::Iterator ArrayList<T>::Iterator::operator++() {
  index_++;
  return *this;
}

template <typename T>
inline typename ArrayList<T>::Iterator ArrayList<T>::Iterator::operator++(int) {
  ArrayList<T>::Iterator it = *this;
  index_++;
  return it;
}

template <typename T>
inline typename ArrayList<T>::Iterator ArrayList<T>::Iterator::operator--() {
  index_--;
  return *this;
}

template <typename T>
inline typename ArrayList<T>::Iterator ArrayList<T>::Iterator::operator--(int) {
  ArrayList<T>::Iterator it = *this;
  index_--;
  return it;
}

template <typename T>
inline bool ArrayList<T>::Iterator::operator==(
    const ArrayList<T>::Iterator& rhs) {
  return index_ == rhs.index_ && offset_ == rhs.offset_ &&
         &array_ == &rhs.array_;
}

template <typename T>
inline bool ArrayList<T>::Iterator::operator!=(
    const ArrayList<T>::Iterator& rhs) {
  return !(*this == rhs);
}

template <typename T>
inline T& ArrayList<T>::Iterator::operator*() {
  return array_.ptr_[index_];
}

template <typename T>
inline ArrayList<T>::ConstIterator::ConstIterator(const ArrayList<T>& array,
                                                  int index, int offset)
    : array_(array) {
  index_ = index;
  offset_ = offset;
}

template <typename T>
inline const T& ArrayList<T>::ConstIterator::Get() const {
  return array_[index_];
}

template <typename T>
inline void ArrayList<T>::ConstIterator::Next() {
  index_ += offset_;
}

template <typename T>
inline bool ArrayList<T>::ConstIterator::HasCurrent() const {
  return (index_ >= 0) && (index_ < array_.size_ - 1);
}

template <typename T>
inline typename ArrayList<T>::ConstIterator
ArrayList<T>::ConstIterator::operator++() {
  index_++;
  return *this;
}

template <typename T>
inline typename ArrayList<T>::ConstIterator
ArrayList<T>::ConstIterator::operator++(int) {
  ArrayList<T>::ConstIterator it = *this;
  index_++;
  return it;
}

template <typename T>
inline typename ArrayList<T>::ConstIterator
ArrayList<T>::ConstIterator::operator--() {
  index_--;
  return *this;
}

template <typename T>
inline typename ArrayList<T>::ConstIterator
ArrayList<T>::ConstIterator::operator--(int) {
  ArrayList<T>::ConstIterator it = *this;
  index_--;
  return it;
}

template <typename T>
inline bool ArrayList<T>::ConstIterator::operator==(
    const ArrayList<T>::ConstIterator& rhs) {
  return index_ == rhs.index_ && offset_ == rhs.offset_ &&
         &array_ == &rhs.array_;
}

template <typename T>
inline bool ArrayList<T>::ConstIterator::operator!=(
    const ArrayList<T>::ConstIterator& rhs) {
  return !(*this == rhs);
}

template <typename T>
inline const T& ArrayList<T>::ConstIterator::operator*() {
  return array_.ptr_[index_];
}

#endif  // ARRAYLIST_HPP_