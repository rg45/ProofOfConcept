#include "stdafx.h"

#include <iostream>
#include <type_traits>

template <typename>
using WeakVoid = void;

template <typename T, typename = void>
struct Is_ : std::false_type { };

template <typename T>
struct Is_<T, std::void_t<decltype(&std::decay_t<T>::operator() < > )>> : std::true_type { };

template <typename T>
using Is = Is_<T>;

void bar()
{
   auto l = [](auto&&...) {};
   using L = decltype(l);
   using V = std::void_t<L>;

   static_assert(Is<L>::value, "Not available");
}

int main()
{
    //TestCallableTraits();
}