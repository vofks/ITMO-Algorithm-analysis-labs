#include <chrono>
#include <fstream>
#include <iostream>

#include "./Sort.hpp";
#include "windows.h"

const char kFileName[] = "results.csv";

int* GetReversedArray(int size) {
  int i = 0;
  int* ptr = new int[size];

  for (; i < size; ++i) {
    ptr[i] = size - i;
  }

  return ptr;
}

int main() {
  const int numOfTests = 10000;
  const int numElements = 40;

  double insertionResult = 0;
  double qLResult = 0;
  double qHResult = 0;
  double tweakedResult = 0;
  double hybridResult = 0;
  double stdResult = 0;

  LARGE_INTEGER Start, End, ElapsedTicks;
  LARGE_INTEGER Frequency;
  QueryPerformanceFrequency(&Frequency);
  double elapsed = 0;

  std::ofstream f;
  f.open(kFileName);

  f << "size,"
    << "ins,"
    << "ql,"
    << "qH,"
    << "tweaked,"
    << "hybrid,"
    << "std\n";

  for (int i = 2; i < numElements; ++i) {
    for (int j = 0; j < numOfTests; ++j) {
      int* ins = GetReversedArray(i);
      int* qL = GetReversedArray(i);
      int* qH = GetReversedArray(i);
      int* tweaked = GetReversedArray(i);
      int* hybrid = GetReversedArray(i);
      int* stdSort = GetReversedArray(i);

      // Insertion sort
      {
        QueryPerformanceCounter(&Start);

        InsertionSort(ins, ins + i - 1,
                      [](const int& a, const int& b) { return a < b; });

        QueryPerformanceCounter(&End);

        ElapsedTicks.QuadPart = End.QuadPart - Start.QuadPart;

        elapsed = double(ElapsedTicks.QuadPart * 1000000) / Frequency.QuadPart;
        insertionResult += elapsed;
      }

      // Quick sort Lomuto partition
      {
        QueryPerformanceCounter(&Start);

        QuickSortL(qL, qL + i - 1,
                   [](const int& a, const int& b) { return a < b; });

        QueryPerformanceCounter(&End);

        ElapsedTicks.QuadPart = End.QuadPart - Start.QuadPart;

        elapsed = double(ElapsedTicks.QuadPart * 1000000) / Frequency.QuadPart;
        qLResult += elapsed;
      }

      // Quick sort Hoare partition
      {
        QueryPerformanceCounter(&Start);

        QuickSortH(qH, qH + i - 1,
                   [](const int& a, const int& b) { return a < b; });

        QueryPerformanceCounter(&End);

        ElapsedTicks.QuadPart = End.QuadPart - Start.QuadPart;

        elapsed = double(ElapsedTicks.QuadPart * 1000000) / Frequency.QuadPart;
        qHResult += elapsed;
      }

      // Tweaked quick sort
      {
        QueryPerformanceCounter(&Start);

        TweakedQuickSort(tweaked, tweaked + i - 1,
                         [](const int& a, const int& b) { return a < b; });

        QueryPerformanceCounter(&End);

        ElapsedTicks.QuadPart = End.QuadPart - Start.QuadPart;

        elapsed = double(ElapsedTicks.QuadPart * 1000000) / Frequency.QuadPart;
        tweakedResult += elapsed;
      }

      // Hybrid tweaked quick sort
      {
        QueryPerformanceCounter(&Start);

        HybridTweakedQuickSort(
            hybrid, hybrid + i - 1,
            [](const int& a, const int& b) { return a < b; });

        QueryPerformanceCounter(&End);

        ElapsedTicks.QuadPart = End.QuadPart - Start.QuadPart;

        elapsed = double(ElapsedTicks.QuadPart * 1000000) / Frequency.QuadPart;
        hybridResult += elapsed;
      }

      // Std quick sort
      {
        QueryPerformanceCounter(&Start);

        std::qsort(stdSort, i, sizeof(int), [](const void* a, const void* b) {
          const int x = *static_cast<const int*>(a);
          const int y = *static_cast<const int*>(b);
          return x - y;
        });

        QueryPerformanceCounter(&End);

        ElapsedTicks.QuadPart = End.QuadPart - Start.QuadPart;

        elapsed = double(ElapsedTicks.QuadPart * 1000000) / Frequency.QuadPart;
        stdResult += elapsed;
      }

      delete[] ins;
      delete[] qL;
      delete[] qH;
      delete[] tweaked;
      delete[] hybrid;
      delete[] stdSort;
    }

    insertionResult /= numOfTests;
    qLResult /= numOfTests;
    qHResult /= numOfTests;
    tweakedResult /= numOfTests;
    hybridResult /= numOfTests;
    stdResult /= numOfTests;

    f << i << "," << insertionResult << "," << qLResult << "," << qHResult
      << "," << tweakedResult << "," << hybridResult << "," << stdResult
      << std::endl;

    insertionResult = 0;
    qLResult = 0;
    qHResult = 0;
    tweakedResult = 0;
    hybridResult = 0;
    stdResult = 0;
  }

  f.close();
}
