#include <iostream>

#include "./Sort.hpp";

int main() {
  int a[] = {9, 3, 9, 4, 1, 4, 2, 4, 7, 0, 0, 8, 1};
  const int size = sizeof(a) / sizeof(a[0]);
  PrintArray(a, size);
  QuickSort(a, a + size - 1, [](int a, int b) { return a < b; });
  PrintArray(a, size);
}
