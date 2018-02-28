﻿#include "stdafx.h"

#include <array>
#include <functional>
#include <iostream>
#include <numeric>
#include <type_traits>
#include <utility>

namespace
{
template <typename, typename = void> struct VectorSize;

template <typename T, size_t N>
struct VectorSize<std::array<T, N>> : std::integral_constant<size_t, N> {};

template <typename T>
constexpr size_t vector_size()
{
   return VectorSize<std::decay_t<T>>::value;
}

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
struct VectorView : T
{
   using value_type = ValueType<T>;

   VectorView() = default;
   VectorView(std::initializer_list<value_type> l) : T{ get<I>(l)... } {}
};

template <typename, typename> struct VectorViewMaker;

template <typename T, size_t...I>
struct VectorViewMaker<T, std::index_sequence<I...>>
{
   using type = VectorView<T, I...>;
};

template <typename T, size_t N = vector_size<T>()>
using VectorViewLite = typename VectorViewMaker<T, std::make_index_sequence<N>>::type;

template <size_t N, typename T = double>
using Vector = VectorViewLite<std::array<T, N>>;

template <typename BinOpT, typename T, typename...U>
struct AccumulatedTypeProvider
{
   using type = decltype(std::declval<BinOpT>()(std::declval<T>(),
      std::declval<typename AccumulatedTypeProvider<BinOpT, U...>::type>()));
};

template <typename BinOpT, typename T>
struct AccumulatedTypeProvider<BinOpT, T>
{
   using type = T;
};

template <typename BinOpT, typename...T>
using AccumulatedType = typename AccumulatedTypeProvider<BinOpT, T...>::type;

template <typename...T>
using ProductType = AccumulatedType<std::multiplies<>, T...>;

template <typename...T>
using SumType = AccumulatedType<std::plus<>, T...>;

template <typename BinOpT, typename T>
T&& accumulate(T&& t)
{
   return std::forward<T>(t);
}

template <typename BinOpT, typename T, typename...U>
AccumulatedType<BinOpT, T, U...> accumulate(T&& t, U&&...u)
{
   return BinOpT()(std::forward<T>(t), accumulate<BinOpT>(std::forward<U>(u)...));
}

template <typename...T>
SumType<T...> sum(T&&...t)
{
   return accumulate<std::plus<>>(std::forward<T>(t)...);
}

template <typename...T>
ProductType<T...> product(T&&...t)
{
   return accumulate<std::multiplies<>>(std::forward<T>(t)...);
}

template <typename T, typename U, size_t...I>
Vector<sizeof...(I), SumType<T, U>> operator + (const VectorView<T, I...>& t, const VectorView<U, I...>& u)
{
   return Vector<sizeof...(I), SumType<T, U>>{(std::forward<T>(t)[I] + std::forward<T>(u)[I])...};
}

template <typename T, typename U, size_t...I>
ProductType<ValueType<T>, ValueType<U>> dot(const VectorView<T, I...>& v, const VectorView<U, I...>& u)
{
   return sum(product(v[I], u[I])...);
}

template <typename T, size_t...I>
std::ostream& operator << (std::ostream& output, const VectorView<T, I...>& v)
{
   output << "[";
   std::initializer_list<int>{(output << " " << v[I], 0)...};
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

   std::cout << Vector<3, Vector<3>>{{1}, {1,2}, {1,2,3}} << std::endl;
}

