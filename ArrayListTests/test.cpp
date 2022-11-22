#include "pch.h"

#include <string>

#include "../ArrayList/ArrayList.hpp"


TEST(ArrayList, Default_Constructor_Creates_Array_With_Zero_Size) {
  const int kDefaultSize = 0;

  ArrayList<int> a;

  ASSERT_EQ(a.Size(), kDefaultSize);
}

TEST(ArrayList, Default_Constructor_Creates_Array_With_Default_Capacity) {
  ArrayList<int> a;

  ASSERT_EQ(a.Capacity(), kDefaultCapacity);
}

TEST(ArrayList, Capacity_Constructor_Creates_Array_With_Zero_Size) {
  const int capacity = 10;

  ArrayList<int> a(capacity);

  ASSERT_EQ(a.Size(), 0);
}

TEST(ArrayList, Capacity_Constructor_Creates_Array_With_Correct_Capacity) {
  const int capacity = 10;

  ArrayList<int> a(capacity);

  ASSERT_EQ(a.Capacity(), capacity);
}

TEST(ArrayList, Copy_Constructor_Creates_Array_With_Correct_Size) {
  ArrayList<int> src(2);
  ArrayList<int> dst(src);

  ASSERT_EQ(src.Size(), dst.Size());
}

TEST(ArrayList, Copy_Constructor_Creates_Array_With_Correct_Capacity) {
  ArrayList<int> src(2);
  ArrayList<int> dst(src);

  ASSERT_EQ(src.Capacity(), dst.Capacity());
}

TEST(ArrayList, Copy_Constructor_Creates_Array_With_Correct_Primitive_Buffer) {
  const int array_size = 100;
  ArrayList<int> src;

  for (int i = 0; i < array_size; ++i) {
    src.Insert(i);
  }

  ArrayList<int> dst(src);

  for (int i = 0; i < array_size; ++i) {
    ASSERT_EQ(dst[i], src[i]);
  }
}

TEST(ArrayList, Copy_Constructor_Creates_Array_With_Correct_Complex_Buffer) {
  const int array_size = 100;
  std::string str =
      "est ullamcorper eget nulla facilisi etiam dignissim diam quis enim "
      "lobortis scelerisque fermentum dui faucibus";
  ArrayList<std::string> src;

  for (int i = 0; i < array_size; ++i) {
    src.Insert(str + std::to_string(i));
  }

  ArrayList<std::string> dst(src);

  for (int i = 0; i < array_size; ++i) {
    ASSERT_EQ(dst[i], src[i]);
  }
}

TEST(ArrayList, Move_Constructor_Creates_Array_With_Correct_Size) {
  const int array_size = 100;
  ArrayList<int> src;

  for (int i = 0; i < array_size; ++i) {
    src.Insert(i);
  }

  int src_size = src.Size();

  ArrayList<int> dst(std::move(src));

  ASSERT_EQ(dst.Size(), src_size);
}

TEST(ArrayList, Move_Constructor_Creates_Array_With_Correct_Capacity) {
  const int array_size = 100;
  ArrayList<int> src;

  for (int i = 0; i < array_size; ++i) {
    src.Insert(i);
  }

  int src_capacity = src.Capacity();

  ArrayList<int> dst(std::move(src));

  ASSERT_EQ(dst.Capacity(), src_capacity);
}

TEST(ArrayList, Move_Constructor_Creates_Array_With_Correct_Primitive_Buffer) {
  const int array_size = 100;
  ArrayList<int> src;

  for (int i = 0; i < array_size; ++i) {
    src.Insert(i);
  }

  ArrayList<int> dst(std::move(src));

  for (int i = 0; i < array_size; ++i) {
    ASSERT_EQ(dst[i], i);
  }
}

TEST(ArrayList, Move_Constructor_Creates_Array_With_Correct_Complex_Buffer) {
  const int array_size = 100;
  std::string str =
      "est ullamcorper eget nulla facilisi etiam dignissim diam quis enim "
      "lobortis scelerisque fermentum dui faucibus";
  ArrayList<std::string> src;

  for (int i = 0; i < array_size; ++i) {
    src.Insert(str + std::to_string(i));
  }

  ArrayList<std::string> dst(std::move(src));

  for (int i = 0; i < array_size; ++i) {
    ASSERT_EQ(dst[i], str + std::to_string(i));
  }
}

TEST(ArrayList, Assignment_Operator_Creates_Array_With_Correct_Size) {
  const int array_size = 100;
  ArrayList<int> src;

  for (int i = 0; i < array_size; ++i) {
    src.Insert(i);
  }

  ArrayList<int> dst = src;

  ASSERT_EQ(dst.Size(), src.Size());
}

TEST(ArrayList, Assignment_Operator_Creates_Array_With_Correct_Capacity) {
  const int array_size = 100;
  ArrayList<int> src;

  for (int i = 0; i < array_size; ++i) {
    src.Insert(i);
  }

  ArrayList<int> dst = src;

  ASSERT_EQ(dst.Capacity(), src.Capacity());
}

TEST(ArrayList,
     Assignment_Operator_Creates_Array_With_Correct_Primitive_Buffer) {
  const int array_size = 100;
  ArrayList<int> src;

  for (int i = 0; i < array_size; ++i) {
    src.Insert(i);
  }

  ArrayList<int> dst = src;

  for (int i = 0; i < array_size; ++i) {
    ASSERT_EQ(dst[i], src[i]);
  }
}

TEST(ArrayList, Assignment_Operator_Creates_Array_With_Correct_Complex_Buffer) {
  const int array_size = 100;
  std::string str =
      "est ullamcorper eget nulla facilisi etiam dignissim diam quis enim "
      "lobortis scelerisque fermentum dui faucibus";
  ArrayList<std::string> src;

  for (int i = 0; i < array_size; ++i) {
    src.Insert(str + std::to_string(i));
  }

  ArrayList<std::string> dst = src;

  for (int i = 0; i < array_size; ++i) {
    ASSERT_EQ(dst[i], src[i]);
  }
}

TEST(ArrayList, Move_Assignment_Operator_Creates_Array_With_Correct_Size) {
  const int array_size = 100;
  ArrayList<int> src;

  for (int i = 0; i < array_size; ++i) {
    src.Insert(i);
  }

  int src_size = src.Size();
  ArrayList<int> dst = std::move(src);

  ASSERT_EQ(dst.Size(), src_size);
}

TEST(ArrayList, Move_Assignment_Operator_Creates_Array_With_Correct_Capacity) {
  const int array_size = 100;
  ArrayList<int> src;

  for (int i = 0; i < array_size; ++i) {
    src.Insert(i);
  }

  int src_capacity = src.Capacity();
  ArrayList<int> dst = std::move(src);

  ASSERT_EQ(dst.Capacity(), src_capacity);
}

TEST(ArrayList,
     Move_Assignment_Operator_Creates_Array_With_Correct_Primitive_Buffer) {
  const int array_size = 100;
  ArrayList<int> src;

  for (int i = 0; i < array_size; ++i) {
    src.Insert(i);
  }

  ArrayList<int> dst = std::move(src);

  for (int i = 0; i < array_size; ++i) {
    ASSERT_EQ(dst[i], i);
  }
}

TEST(ArrayList,
     Move_Assignment_Operator_Creates_Array_With_Correct_Complex_Buffer) {
  const int array_size = 100;
  std::string str =
      "est ullamcorper eget nulla facilisi etiam dignissim diam quis enim "
      "lobortis scelerisque fermentum dui faucibus";
  ArrayList<std::string> src;

  for (int i = 0; i < array_size; ++i) {
    src.Insert(str + std::to_string(i));
  }

  ArrayList<std::string> dst = std::move(src);

  for (int i = 0; i < array_size; ++i) {
    ASSERT_EQ(dst[i], str + std::to_string(i));
  }
}

TEST(ArrayList, Push_Back_Icreases_Size) {
  ArrayList<int> src;

  src.Insert(5);
  ASSERT_EQ(src.Size(), 1);

  src.Insert(3);
  ASSERT_EQ(src.Size(), 2);
}

TEST(ArrayList, Insert_Icreases_Size) {
  ArrayList<int> src;

  src.Insert(12);
  src.Insert(0, 3);

  ASSERT_EQ(src.Size(), 2);
}

TEST(ArrayList, Can_Push_Back_Primitive_Type) {
  ArrayList<int> src;

  src.Insert(3);
  src.Insert(4);

  ASSERT_EQ(src[1], 4);
}

TEST(ArrayList, Can_Insert_Primitive_Type) {
  ArrayList<int> src;

  src.Insert(10);
  src.Insert(0, 12);

  ASSERT_EQ(src[0], 12);
}

TEST(ArrayList, Can_Push_Back_Complex_Type) {
  const std::string str1 =
      "est ullamcorper eget nulla facilisi etiam dignissim diam quis enim "
      "lobortis scelerisque fermentum dui faucibus";
  const std::string str2 = "hello";
  ArrayList<std::string> src;

  src.Insert(str1);
  src.Insert(str2);

  ASSERT_EQ(src[1], str2);
}

TEST(ArrayList, Can_Insert_Complex_Type) {
  const std::string str1 =
      "est ullamcorper eget nulla facilisi etiam dignissim diam quis enim "
      "lobortis scelerisque fermentum dui faucibus";
  const std::string str2 = "hello";
  ArrayList<std::string> src;

  src.Insert(str2);
  src.Insert(0, str1);

  ASSERT_EQ(src[0], str1);
}

TEST(ArrayList, Insert_To_Full_List_Doubles_Capacity) {
  const std::string str =
      "est ullamcorper eget nulla facilisi etiam dignissim diam quis enim "
      "lobortis scelerisque fermentum dui faucibus";
  const int capacity = 1;
  ArrayList<std::string> src(capacity);

  src.Insert("hello");
  src.Insert(str);

  ASSERT_EQ(src.Capacity(), capacity * 2);
}

TEST(ArrayList, Insert_To_Incorrect_Index_Returns_Error_Value) {
  const int error_val = -1;
  ArrayList<int> a(10);

  ASSERT_EQ(a.Insert(-1, 5), error_val);
  ASSERT_EQ(a.Insert(1000, 4), error_val);
}

TEST(ArrayList, Can_Remove_Primitive_Element) {
  const int array_size = 100;
  const int index_to_remove = 2;
  ArrayList<int> a;

  for (int i = 0; i < array_size; ++i) {
    a.Insert(i);
  }

  int removed = a[index_to_remove];
  a.Remove(index_to_remove);

  ASSERT_EQ(a.Size(), array_size - 1);
  ASSERT_NE(a[index_to_remove], removed);
}

TEST(ArrayList, Can_Remove_Complex_Element) {
  const std::string str =
      "est ullamcorper eget nulla facilisi etiam dignissim diam quis enim "
      "lobortis scelerisque fermentum dui faucibus";
  const int array_size = 100;
  const int index_to_remove = 2;
  ArrayList<std::string> a;

  for (int i = 0; i < array_size; ++i) {
    a.Insert(str + std::to_string(i));
  }

  std::string removed = a[index_to_remove];
  a.Remove(index_to_remove);

  ASSERT_EQ(a.Size(), array_size - 1);
  ASSERT_NE(a[index_to_remove], removed);
}

TEST(ArrayList, Can_Not_Remove_At_Incorrect_Index) {
  ASSERT_NO_THROW({
    ArrayList<int> a;
    a.Remove(-23);
    a.Remove(0);
    a.Remove(100);
  });
}

TEST(ArrayList, Supports_Range_For) {
  const int array_size = 100;
  ArrayList<int> src;

  for (int i = 0; i < array_size; ++i) {
    src.Insert(i);
  }

  int i = 0;
  for (const auto &e : src) {
    ASSERT_EQ(e, src[i]);
    ++i;
  }
}

TEST(Iterator, Can_Get_Iterator) {
  const int array_size = 100;
  ArrayList<int> a;

  for (int i = 0; i < array_size; ++i) {
    a.Insert(i);
  }

  ArrayList<int>::Iterator it = a.iterator();
  int i = 0;

  for (; it.HasCurrent(); it.Next()) {
    ASSERT_EQ(it.Get(), i);
    ++i;
  }
}

TEST(Iterator, Can_Set_Value) {
  const int array_size = 10;
  const int new_value = 333;
  ArrayList<int> a;

  for (int i = 0; i < array_size; ++i) {
    a.Insert(a[i]);
  }

  ArrayList<int>::Iterator it = a.iterator();
  it.Next();
  it.Set(new_value);

  ASSERT_EQ(a[1], new_value);
}

TEST(Iterator, Can_Get_ReverseIterator) {
  const int array_size = 100;
  ArrayList<int> a;

  for (int i = 0; i < array_size; ++i) {
    a.Insert(i);
  }

  ArrayList<int>::Iterator it = a.reverseIterator();
  int i = array_size - 1;

  for (; it.HasCurrent(); it.Next()) {
    ASSERT_EQ(it.Get(), i);
    --i;
  }
}

TEST(Iterator, Can_Pre_Increment) {
  const int array_size = 100;
  ArrayList<int> a;

  for (int i = 0; i < array_size; ++i) {
    a.Insert(i);
  }

  ArrayList<int>::Iterator it = a.iterator();
  int i = 0;

  for (; it.HasCurrent(); ++it) {
    ASSERT_EQ(it.Get(), i);
    ++i;
  }
}

TEST(Iterator, Can_Post_Increment) {
  const int array_size = 100;
  ArrayList<int> a;

  for (int i = 0; i < array_size; ++i) {
    a.Insert(i);
  }

  ArrayList<int>::Iterator it = a.iterator();
  int i = 0;

  for (; it.HasCurrent(); it++) {
    ASSERT_EQ(it.Get(), i);
    ++i;
  }
}

TEST(Iterator, Can_Pre_Decrement) {
  const int array_size = 100;
  ArrayList<int> a;

  for (int i = 0; i < array_size; ++i) {
    a.Insert(i);
  }

  ArrayList<int>::Iterator it = a.reverseIterator();
  int i = array_size - 1;

  for (; it.HasCurrent(); --it) {
    ASSERT_EQ(it.Get(), i);
    --i;
  }
}

TEST(Iterator, Can_Post_Decrement) {
  const int array_size = 100;
  ArrayList<int> a;

  for (int i = 0; i < array_size; ++i) {
    a.Insert(i);
  }

  ArrayList<int>::Iterator it = a.reverseIterator();
  int i = array_size - 1;

  for (; it.HasCurrent(); it--) {
    ASSERT_EQ(it.Get(), i);
    --i;
  }
}

TEST(ConstIterator, Can_Get_Iterator) {
  const int array_size = 100;
  ArrayList<int> a;

  for (int i = 0; i < array_size; ++i) {
    a.Insert(i);
  }

  const ArrayList<int> b(a);

  ArrayList<int>::ConstIterator it = b.iterator();
  int i = 0;

  for (; it.HasCurrent(); it.Next()) {
    ASSERT_EQ(it.Get(), i);
    ++i;
  }
}

TEST(ConstIterator, Can_Get_ReverseIterator) {
  const int array_size = 100;
  ArrayList<int> a;

  for (int i = 0; i < array_size; ++i) {
    a.Insert(i);
  }

  const ArrayList<int> b(a);

  ArrayList<int>::ConstIterator it = b.reverseIterator();
  int i = array_size - 1;

  for (; it.HasCurrent(); it.Next()) {
    ASSERT_EQ(it.Get(), i);
    --i;
  }
}

TEST(ConstIterator, Can_Pre_Increment) {
  const int array_size = 100;
  ArrayList<int> a;

  for (int i = 0; i < array_size; ++i) {
    a.Insert(i);
  }

  const ArrayList<int> b(a);

  ArrayList<int>::ConstIterator it = b.iterator();
  int i = 0;

  for (; it.HasCurrent(); ++it) {
    ASSERT_EQ(it.Get(), i);
    ++i;
  }
}

TEST(ConstIterator, Can_Post_Increment) {
  const int array_size = 100;
  ArrayList<int> a;

  for (int i = 0; i < array_size; ++i) {
    a.Insert(i);
  }

  const ArrayList<int> b(a);

  ArrayList<int>::ConstIterator it = b.iterator();
  int i = 0;

  for (; it.HasCurrent(); it++) {
    ASSERT_EQ(it.Get(), i);
    ++i;
  }
}

TEST(ConstIterator, Can_Pre_Decrement) {
  const int array_size = 100;
  ArrayList<int> a;

  for (int i = 0; i < array_size; ++i) {
    a.Insert(i);
  }

  const ArrayList<int> b(a);

  ArrayList<int>::ConstIterator it = b.reverseIterator();
  int i = array_size - 1;

  for (; it.HasCurrent(); --it) {
    ASSERT_EQ(it.Get(), i);
    --i;
  }
}

TEST(ConstIterator, Can_Post_Decrement) {
  const int array_size = 100;
  ArrayList<int> a;

  for (int i = 0; i < array_size; ++i) {
    a.Insert(i);
  }

  const ArrayList<int> b(a);

  ArrayList<int>::ConstIterator it = b.reverseIterator();
  int i = array_size - 1;

  for (; it.HasCurrent(); it--) {
    ASSERT_EQ(it.Get(), i);
    --i;
  }
}