#include <iostream>

#include "./Sort.hpp";

int main() {
  int a[] = {9, 9, 9, 0, 3, 3, 2, 1};
  const int size = sizeof(a) / sizeof(a[0]);
  PrintArray(a, size);
  QuickSort(a, a + size - 1, [](int a, int b) { return a < b; });
  PrintArray(a, size);

  
}
