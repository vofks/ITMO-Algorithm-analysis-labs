#include "pch.h"

#include <random>

#include "../Sort/Sort.hpp"


TEST(Median, Median_Performs_Correct_Sort) {
  int a[] = {1, 2, 3};
  int b[] = {3, 2, 1};
  int c[] = {3, 3, 1};
  int d[] = {1, 3, 3};
  int e[] = {4, 4, 3, 1};

  const int size = sizeof(a) / sizeof(a[0]);

  int* pivot = Median(a, a + size - 1, [](int a, int b) { return a < b; });
  ASSERT_EQ(*pivot, 2);

  pivot = Median(b, b + size - 1, [](int a, int b) { return a < b; });
  ASSERT_EQ(*pivot, 2);
  ASSERT_EQ(b[0], 1);
  ASSERT_EQ(b[2], 3);

  pivot = Median(c, c + size - 1, [](int a, int b) { return a < b; });
  ASSERT_EQ(*pivot, 3);
  ASSERT_EQ(c[0], 1);
  ASSERT_EQ(c[2], 3);

  pivot = Median(d, d + size - 1, [](int a, int b) { return a < b; });
  ASSERT_EQ(*pivot, 3);
  ASSERT_EQ(d[0], 1);
  ASSERT_EQ(d[2], 3);

  pivot = Median(e, e + 3, [](int a, int b) { return a < b; });
  ASSERT_EQ(*pivot, 4);
  ASSERT_EQ(e[0], 1);
  ASSERT_EQ(e[3], 4);
}

TEST(InsertionSort, Sorts_Correctly) {
  std::mt19937 gen(time(0));
  const int size = 100000;
  int* ptr = new int[size];

  int i = 0;
  for (; i < size; ++i) {
    ptr[i] = gen();
  }

  InsertionSort(ptr, ptr + size - 1,
                [](const int& a, const int& b) { return a < b; });
  i = 0;
  for (; i < size - 1; ++i) {
    ASSERT_GE(ptr[i + 1], ptr[i]);
  }

  delete[] ptr;
}

TEST(QuickSort, Sorts_Correctly) {
  std::mt19937 gen(time(0));
  const int size = 100000;
  int* ptr = new int[size];

  int i = 0;
  for (; i < size; ++i) {
    ptr[i] = gen();
  }

  QuickSort(ptr, ptr + size - 1,
                   [](const int& a, const int& b) { return a < b; });
  i = 0;
  for (; i < size - 1; ++i) {
    ASSERT_GE(ptr[i + 1], ptr[i]);
  }

  delete[] ptr;
}

TEST(TweakedQuickSort, Sorts_Correctly) {
  std::mt19937 gen(time(0));
  const int size = 100000;
  int* ptr = new int[size];

  int i = 0;
  for (; i < size; ++i) {
    ptr[i] = gen();
  }

  TweakedQuickSort(ptr, ptr + size - 1,
                [](const int& a, const int& b) { return a < b; });
  i = 0;
  for (; i < size - 1; ++i) {
    ASSERT_GE(ptr[i + 1], ptr[i]);
  }

  delete[] ptr;
}
