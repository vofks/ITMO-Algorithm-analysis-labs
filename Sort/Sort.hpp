#ifndef SORT_HPP_

#include <utility>

const int kLengthThreshold = 4;

template <typename T>
void PrintArray(T* ptr, int size) {
  int i = 0;
  for (; i < size; ++i) std::cout << ptr[i] << " ";
  std::cout << std::endl;
}

template <typename T, typename TComapre>
T Median(
    const T& first, const T& middle, const T& last,
    TComapre comp) {  // TODO: (Bug) incorrect median on (3,3,2) and similar
  if (comp(first, middle) && comp(last, first) ||
      comp(first, last) && comp(middle, first)) {
    return first;
  } else if (comp(middle, first) && comp(last, middle) ||
             comp(middle, last) && comp(first, middle)) {
    return middle;
  }

  return last;
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
T* Partition(T* start, T* end, TCompare comp) {
  T pivot = std::move(Median(*start, *(start + (end - start) / 2), *end, comp));

  T* i = start;
  T* j = end - 1;

  while (i < j) {
    while (comp(*i, pivot)) ++i;
    while (comp(pivot, *j)) --j;
  }
}

template <typename T, typename TCompare>
void QuickSort(T* start, T* end, TCompare comp) {
  if (start < end) {
    if (end - start < kLengthThreshold) {
      InsertionSort(start, end, comp);

      return;
    }

    T pivot =
        std::move(Median(*start, *(start + (end - start) / 2), *end, comp));

    T* i = start;
    T* j = end;

    while (i < j) {
      while (comp(*i, pivot)) ++i;
      while (comp(pivot, *j)) --j;

      if (*i != *j) {
        std::swap(*i, *j);
      }

      ++i;
      --j;

      if (end - j > j - start) {
        QuickSort(start, j, comp);
        start = j + 1;
      } else {
        QuickSort(j + 1, end, comp);
        end = j;
      }
    }
  }
}

int partition(int array[], int low, int high) {
  int pivot = array[high];
  int i = (low - 1);

  for (int j = low; j < high; j++) {
    if (array[j] <= pivot) {
      i++;

      std::swap(array[i], array[j]);
    }
  }

  std::swap(array[i + 1], array[high]);

  return (i + 1);
}

void quickSort(int array[], int low, int high) {
  if (low < high) {
    int pi = partition(array, low, high);

    quickSort(array, low, pi - 1);
    quickSort(array, pi + 1, high);
  }
}

#define SORT_HPP_
#endif  // SORT_HPP_
