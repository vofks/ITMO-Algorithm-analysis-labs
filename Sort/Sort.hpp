#ifndef SORT_HPP_

#include <utility>

const int kLengthThreshold = 4;

template <typename T>
void PrintArray(T* ptr, int size) {
  int i = 0;
  for (; i < size; ++i) std::cout << ptr[i] << " ";
  std::cout << std::endl;
}

template <typename T, typename TCompare>
T* Median(T* low, T* high, TCompare comp) {
  T* middle = low + (high - low) / 2;

  if (comp(*high, *low)) std::swap(*high, *low);
  if (comp(*middle, *low)) std::swap(*middle, *low);
  if (comp(*high, *middle)) std::swap(*middle, *high);

  return middle;
}

template <typename T, typename TCompare>
void InsertionSort(T* start, T* end, TCompare comp) {
  if (start == end) {
    return;
  }

  T* i = start + 1;
  T* j = nullptr;
  T key(*i);

  for (; i <= end; ++i) {
    key = *i;
    j = i;

    while (j > start && comp(key, *(j - 1))) {
      *j = std::move(*(j - 1));
      --j;
    }

    (*j) = std::move(key);
  }
}

template <typename T, typename TCompare>
T* Partition(T* low, T* high, TCompare comp) {
  T* middle = low + (high - low) / 2;

  if (comp(*high, *low)) std::swap(*high, *low);
  if (comp(*middle, *low)) std::swap(*middle, *low);
  if (comp(*high, *middle)) std::swap(*high, *middle);

  std::swap(*middle, *(high - 1));
  T pivot = *(high - 1);

  T* i = low - 1;
  T* j = high + 1;

  while (true) {
    while (comp(*(++i), pivot))
      ;
    while (comp(pivot, *(--j)))
      ;

    if (i >= j) {
      return j;
    }

    std::swap(*i, *j);
  }
}

template <typename T, typename TCompare>
T* HoarePartition(T* low, T* high, TCompare comp) {
  T pivot = *low;
  T* i = low - 1;
  T* j = high + 1;

  while (true) {
    do {
      ++i;
    } while (comp(*i, pivot));
    do {
      --j;
    } while (comp(pivot, *j));

    if (i >= j) return j;

    std::swap(*i, *j);
  }
}

template <typename T, typename TCompare>
T* LomutoPartition(T* low, T* high, TCompare comp) {
  T* pivot = high;
  T* i = low - 1;
  T* j = low;
  for (; j < high; ++j) {
    if (comp(*j, *pivot)) {
      ++i;
      std::swap(*i, *j);
    }
  }

  std::swap(*(i + 1), *high);

  return i + 1;
}

template <typename T, typename TCompare>
void QuickSort(T* low, T* high, TCompare comp) {
  if (low < high) {
    /*if (high - low < kLengthThreshold) {
      InsertionSort(low, high, comp);
      return;
    }*/

    T* pi = HoarePartition(low, high, comp);

    QuickSort(low, pi - 1, comp);
    QuickSort(pi + 1, high, comp);
  }
}

#define SORT_HPP_
#endif  // SORT_HPP_
