#include "stdafx.h"

#include <functional>
#include <numeric>
#include <iostream>
#include <tuple>
#include <vector>

namespace
{

template <typename...T>
class ZipIterator
{
public:

   using reference = std::tuple<decltype(*std::declval<T>())...>;

public:

   ZipIterator(T&&...i) : m(std::forward<T>(i)...) { };

   reference operator * () const { return std::tie(*std::get<T>(m)...); }
   ZipIterator operator ++ () { char c[]{ (++std::get<T>(m), 0)... }; return *this; }

   bool operator == (const ZipIterator& other) const {
      const bool eq[]{ (std::get<T>(m) == std::get<T>(other.m))... };
      return std::accumulate(std::begin(eq), std::end(eq), false, std::logical_or<>());
   }
   bool operator != (const ZipIterator& other) const { return !(*this == other); }

private:
   std::tuple<T...> m;
};

template <typename T>
struct IteratorRange
{
   T m_begin;
   T m_end;

   const T& begin() const { return m_begin; }
   const T& end() const { return m_end; }
};

template <typename...T>
using ZipRange = IteratorRange<ZipIterator<decltype(std::begin(std::declval<T>()))...>>;

template <typename...T>
ZipRange<T...> make_zip_range(T&&...t) { return {{std::begin(t)...},{ std::end(t)...} }; }


template <typename T>
class C {
public:
   void memberFunction() {}

   template <typename S>
   void templateMemberFunction() {}
};

} // namespace

void TestZipRange()
{
   std::vector<int> a { 1, 2, 3 };
   std::vector<std::string> b { "hello", "world", "foo", "bar" };
   int c[] { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };

   for (auto&& entry : make_zip_range(a, b, c))
   {
      std::cout << std::get<0>(entry) << ", " << std::get<1>(entry) << ", " << std::get<2>(entry) << std::endl;
   }
}

