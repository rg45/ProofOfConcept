#include "stdafx.h"

void CompileTimeStrings();
void EmplaceBackTest();
void SmartEnums1();
void SmartEnums2();
void CTCTest();

template <typename>
struct Base {};

template <typename T>
std::true_type aux(const Base<T>&);
std::false_type aux(...);

template <typename T>
constexpr bool test() { return decltype(aux(std::declval<T>()))::value; }

template <typename T>
struct A
{
   operator Base<T>&() const;
};

template <typename T>
struct B : A<T> { };

template struct B<int>;

int main()
{
   std::cout << std::boolalpha << std::endl;

   TRACE(test<int>());
   TRACE(test<class X>());
   TRACE(test<A<int>>());
}
