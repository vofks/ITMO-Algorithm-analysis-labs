#include <iostream>

#include "./Sort.hpp";

int main() {
  int a[] = {3, 3, 0, 6, 5, 1, 4, 4, 9, 7};
  int size = sizeof(a) / sizeof(a[0]);
  PrintArray(a, size);
  QuickSort(a, a + size - 1, [](int a, int b) { return a < b; });
  PrintArray(a, size);
}
