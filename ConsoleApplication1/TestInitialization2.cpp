#include "stdafx.h"

#include <array>

namespace
{
template <typename T, size_t...I>
class Array
{
   template <size_t> using Arg = T;
public:

   Array() = default;
   Array(const Arg<I>&...t) : m{ t... } {}

   const T& operator[](size_t i) const { return m[i]; }

   template <size_t J>
   friend T&& get(Array&& a) { return get<J>(std::move(a).m); }

   template <size_t J>
   friend T& get(Array& a) { return get<J>(a.m); }

   template <size_t J>
   friend const T& get(const Array& a) { return get<J>(a.m); }

private:
   std::array<T, sizeof...(I)> m;
};

template <typename, typename> struct ArrayMaker;
template <typename T, size_t...I>
struct ArrayMaker<T, std::index_sequence<I...>>
{
   using type = Array<T, I...>;
};

template <typename T, size_t N>
using ArrayLite = typename ArrayMaker<T, std::make_index_sequence<N>>::type;

template <typename T, size_t...I>
std::ostream& operator << (std::ostream& output, const Array<T, I...>& array)
{
   const char* delim[] = {"[", " ", " ]"};
   output << delim[0];
   char c[]{(output << delim[1] << get<I>(array), 0)...};
   return output << delim[2];
}

} // namespace

void TestInitialization2()
{
   ArrayLite<ArrayLite<ArrayLite<int, 3>, 3>, 3> a
   {
      { { 0,1,2 },{ 0,1,2 },{ 0,1,3 } },
      { { 0,1,2 },{ 0,1,2 },{ 0,1,3 } },
      { { 0,1,2 },{ 0,1,2 },{ 0,1,3 } }
   };
   std::cout << get<1>(a) << std::endl;
}

