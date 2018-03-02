#include "stdafx.h"

#include <array>
#include <initializer_list>
#include <iostream>
#include <iterator>

namespace
{

template <typename T, size_t N>
struct Array
{
public:

   Array() = default;
   Array(std::initializer_list<T> init) : Array(init, std::make_index_sequence<N>()) {}

   auto begin() const { return std::begin(data); }
   auto end() const { return std::end(data); }

private:
   template <size_t...I>
   Array(std::initializer_list<T> init, std::index_sequence<I...>, T def_value = T())
      : data{ get<I>(init, def_value)... } { }

   template <size_t I>
   static const T& get(const std::initializer_list<T>& init, const T& def_value) {
      return I < init.size() ? init.begin()[I] : def_value;
   }

private:
   std::array<T, N> data;
   // T data[N];
};

template <typename T, size_t N>
std::ostream& operator << (std::ostream& output, const Array<T, N>& array)
{
   output << "[";
   for (auto&& element : array)
      output << " " << element;

   return output << " ]";
}

} // namespace

void TestInitialization()
{
   Array<Array<int, 3>, 3> m{ { 0, 1 },{ 2, 3 } };
   std::cout << m << std::endl;
}

