#include "pch.h"

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