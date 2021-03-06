﻿#include "stdafx.h"

#include "accumulate.h"
#include <array>
#include <iostream>

namespace
{
template <typename, typename = void> struct VectorSize;

template <typename T, size_t N>
struct VectorSize<std::array<T, N>> : std::integral_constant<size_t, N> {};

template <typename T, size_t N>
struct VectorSize<T[N]> : std::integral_constant<size_t, N> {};

template <typename T>
constexpr size_t vector_size = VectorSize<std::decay_t<T>>::value;

template <typename T, typename = void>
struct ValueTypeAccessor
{
   using type = typename std::decay_t<T>::value_type;
};

template <typename T>
using ValueType = typename ValueTypeAccessor<T>::type;

template <size_t I, typename T>
constexpr const T& get(const std::initializer_list<T>& l, const T& def = T())
{
   return I < size(l) ? begin(l)[I] : def;
}

template <typename T, size_t...I>
class VectorView
{
public:
   using value_type = ValueType<T>;
   template <size_t> using Arg = value_type;

   VectorView() = default;
   VectorView(const Arg<I>&...t) : m{ t... } {}

   template <size_t J>
   friend value_type&& get(VectorView&& v) { return get<J>(std::move(v).m); }

   template <size_t J>
   friend value_type& get(VectorView& v) { return get<J>(v.m); }

   template <size_t J>
   friend const value_type& get(const VectorView& v) { return get<J>(v.m); }

private:
   T m;
};

template <typename, typename> struct VectorViewMaker;

template <typename T, size_t...I>
struct VectorViewMaker<T, std::index_sequence<I...>>
{
   using type = VectorView<T, I...>;
};

template <typename T, size_t N = vector_size<T>>
using VectorViewLite = typename VectorViewMaker<T, std::make_index_sequence<N>>::type;

template <size_t N, typename T = double>
using Vector = VectorViewLite<std::array<T, N>>;

template <typename T, typename U, size_t...I>
ProductType<ValueType<T>, ValueType<U>> dot(const VectorView<T, I...>& v, const VectorView<U, I...>& u)
{
   return sum(product(get<I>(v), get<I>(u))...);
}

template <typename T, size_t...I>
std::ostream& operator << (std::ostream& output, const VectorView<T, I...>& v)
{
   output << "[";
   std::initializer_list<int>{(output << " " << get<I>(v), 0)...};
   return output << " ]";
}

} // namespace

void TestLinalg()
{
   const Vector<3> v1 = { 1, 2, 3 };
   const Vector<3> v2 = { 10, 20, 30 };
   const auto res = dot(v1, v2);
   std::cout << res << std::endl;
   std::cout << v1 << std::endl;

   std::cout << Vector<3, Vector<3>>{{1, 2, 3}, {1,2,3}, {1,2,3}} << std::endl;
}

