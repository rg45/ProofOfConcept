#include "stdafx.h"

#include <iostream>
#include <type_traits>

#define TRACE(exp) (std::cout << #exp " = " << (exp) << std::endl)

#define IS_COMPLETE(T) IsComplete<T, __COUNTER__>::value

template <typename, int, typename = size_t>
struct IsComplete : std::false_type {};

template <typename T, int C>
struct IsComplete<T, C, decltype(sizeof(T))> : std::true_type {};

class A;
constexpr auto c1 = IS_COMPLETE(A);
class A {};
constexpr auto c2 = IS_COMPLETE(A);

using CTCInt = int;

#define CREATE_CTC(name) \
  template <CTCInt> struct name; \
  template <> struct name<0> {};

void CTCTest()
{
   std::cout << std::boolalpha;

   TRACE(IS_COMPLETE(int));
   TRACE(IS_COMPLETE(void));
   TRACE(IS_COMPLETE(class X));
   class X {};
   TRACE(IS_COMPLETE(class X));
   TRACE(c1);
   TRACE(c2);
}
