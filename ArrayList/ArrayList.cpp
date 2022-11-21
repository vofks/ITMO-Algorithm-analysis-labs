#include "./ArrayList.hpp"

template <typename T>
ArrayList<T>::Iterator::Iterator(const ArrayList<T>& array, int index,
                                 int offset) {
  array_ = array;
  index_ = index;
  offset_ = offset;
}

template <typename T>
T& ArrayList<T>::Iterator::Get() const {
  return array_[index_];
}

template <typename T>
void ArrayList<T>::Iterator::Set(const T& value) {
  array_[index_] = value;
}

template <typename T>
void ArrayList<T>::Iterator::Next() {
  index_ += offset_;
}

template <typename T>
bool ArrayList<T>::Iterator::HasNext() const {
  int next_index = index_ + offset_;
  return (next_index >= 0) && (next_index < array_.size_);
}

template <typename T>
typename ArrayList<T>::Iterator ArrayList<T>::Iterator::operator++() {
  index_++;
  return *this;
}

template <typename T>
typename ArrayList<T>::Iterator ArrayList<T>::Iterator::operator++(int) {
  ArrayList<T>::Iterator it = *this;
  index_++;
  return it;
}

template <typename T>
typename ArrayList<T>::Iterator ArrayList<T>::Iterator::operator--() {
  index_--;
  return *this;
}

template <typename T>
typename ArrayList<T>::Iterator ArrayList<T>::Iterator::operator--(int) {
  ArrayList<T>::Iterator it = *this;
  index_--;
  return it;
}

template <typename T>
ArrayList<T>::ConstIterator::ConstIterator(const ArrayList<T>& array, int index,
                                           int offset) {
  array_ = array;
  index_ = index;
  offset_ = offset;
}

template <typename T>
const T& ArrayList<T>::ConstIterator::Get() const {
  return array_[index_];
}

template <typename T>
void ArrayList<T>::ConstIterator::Next() {
  index_ += offset_;
}

template <typename T>
bool ArrayList<T>::ConstIterator::HasNext() const {
  int next_index = index_ + offset_;
  return (next_index >= 0) && (next_index < array_.size_);
}

template <typename T>
typename ArrayList<T>::ConstIterator ArrayList<T>::ConstIterator::operator++() {
  index_++;
  return *this;
}

template <typename T>
typename ArrayList<T>::ConstIterator ArrayList<T>::ConstIterator::operator++(
    int) {
  ArrayList<T>::ConstIterator it = *this;
  index_++;
  return it;
}

template <typename T>
typename ArrayList<T>::ConstIterator ArrayList<T>::ConstIterator::operator--() {
  index_--;
  return *this;
}

template <typename T>
typename ArrayList<T>::ConstIterator ArrayList<T>::ConstIterator::operator--(
    int) {
  ArrayList<T>::ConstIterator it = *this;
  index_--;
  return it;
}
