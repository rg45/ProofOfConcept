#include "stdafx.h"

#include <iostream>
#include <string>
#include <vector>

void EmplaceBackTest()
{
   std::vector<std::string> v { "Hello" };

   while (v.size() < 100)
   {
      v.reserve(v.size() + 1); // Comment this to obtain UB
      v.emplace_back(v[0]);
      std::cout << v.size() << ": " << v.back() << std::endl;
   }
}
