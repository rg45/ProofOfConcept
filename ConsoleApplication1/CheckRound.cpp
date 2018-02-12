#include "stdafx.h"

#include <iostream>
#include <cmath>
#include <limits>
#include <iomanip>

namespace
{

template <typename I, typename F>
void checkRound(F f)
{
   const auto actual = I(std::round(f));
   const auto expected = I(std::lround(f));

   if (actual != expected)
   {
      std::cout << "Wrong rounding: " << f << " -> " << actual << std::endl;
   }
}

template <typename I, typename F>
void checkRound()
{
   using Limits = std::numeric_limits<I>;

   std::cout << std::setprecision(Limits::digits10 + 3) << std::endl;

   for (auto i = Limits::min(); i < Limits::max(); ++i)
   {
      if (i % 10000000 == 0)
      {
         std::cout << i << std::endl;
      }
      checkRound<I>(F(i + 0.1));
      checkRound<I>(F(i + 0.9));
   }
}

} // namespace

void CheckRound()
{
   checkRound<int, float>();
}

