#include "stdafx.h"

template <typename T>
void testForwardingRef(T&& t)
{
   (t);
   std::cout << poc::utility::GetTypeName<T>() << std::endl;
   std::cout << poc::utility::GetTypeName<decltype(t)>() << std::endl;
   std::cout << poc::utility::GetTypeName<decltype((t))>() << std::endl;
}

int main()
{
   testForwardingRef(int(42));
}
