#include <iostream>

#include "./Sort.hpp";

int main() {
  int a[] = {4, 2, 1, 7, 9, 0, 0, 3};
  int b[] = {3, 1, 2, 0};
  int c[] = {2, 1, 7, 6, 5, 4, 0, 3};
  PrintArray(a, 8);

  InsertionSort(a, &a[7], [](int a, int b) { return a < b; });

  std::cout << Median(3, 3, 2, [](int a, int b) { return (a < b); });
  std::cout << std::endl;

  PrintArray(a, 8);

  PrintArray(b, 4);

  QuickSort(b, &b[3], [](int a, int b) { return a < b; });

  PrintArray(b, 4);

  PrintArray(c, 8);

  QuickSort(c, &c[7], [](int a, int b) { return a < b; });

  PrintArray(c, 8);
}
