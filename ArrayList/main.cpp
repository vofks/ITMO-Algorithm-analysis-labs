#include "ArrayList.hpp"

#include <iostream>

class Foo {
 public:
  class Bar;

  int A() const { return a; }

  Bar GetBar();

 private:
  int a = 0;
};

class Foo::Bar {
 public:
  Bar(Foo* value) { ptr = value; }

  void ChangeFooA(int value) { ptr->a = value; }

 private:
  int b = 0;
  Foo* ptr = nullptr;
};

Foo::Bar Foo::GetBar() { return Foo::Bar(this); }

int main() {
  /*Foo a;
  Foo::Bar b = a.GetBar();
  b.ChangeFooA(3);
  std::cout << a.A();
  std::cout << a.a;*/
}
