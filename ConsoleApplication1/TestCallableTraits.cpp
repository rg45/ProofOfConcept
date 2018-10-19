#include "stdafx.h"

#include <tuple>
#include <type_traits>

namespace
{

/// @class CallableTraits is a common traits class for various instances:
/// ordinary functions, function objects and lambdas
template <typename>
struct CallableTraits;

template <typename ReturnT, typename...ParamT>
struct CallableTraits<ReturnT(ParamT...)>
{
   static constexpr size_t Arity = sizeof...(ParamT);
   using Signature = ReturnT(ParamT...);
   using ReturnType = ReturnT;
   using ParameterTypes = std::tuple<ParamT...>;
};

template <typename ReturnT, typename...ParamT>
struct CallableTraits<ReturnT(*)(ParamT...)> : CallableTraits<ReturnT(ParamT...)> {};

template <typename T, typename ReturnT, typename...ParamT>
struct CallableTraits<ReturnT(T::*)(ParamT...)> : CallableTraits<ReturnT(ParamT...)> {};

template <typename T, typename ReturnT, typename...ParamT>
struct CallableTraits<ReturnT(T::*)(ParamT...) const> : CallableTraits<ReturnT(ParamT...)> {};

template <typename T>
struct CallableTraits : CallableTraits<decltype(&std::decay<T>::type::operator())> {};

///@brief Get callable arity
template <typename T>
constexpr size_t CallableArity = CallableTraits<T>::Arity;

///@brief Get callable signature
template <typename T>
using CallableSignature = typename CallableTraits<T>::Signature;

///@brief Get callable return type
template <typename T>
using CallableReturnType = typename CallableTraits<T>::ReturnType;

///@brief Get callable parameter type by its order index
template <typename T, size_t I>
using CallableParameterType = typename std::tuple_element<I, typename CallableTraits<T>::ParameterTypes>::type;

} // namespace

void TestCallableTraits()
{
   int i = 42;

   auto lambda = [=](int, double, const std::string&) -> decltype(auto) { return (i); };
   using LambdaType = decltype(lambda);

   static_assert(CallableArity<LambdaType> == 3, "");
   static_assert(std::is_same<int, CallableParameterType<LambdaType, 0>>::value, "");
   static_assert(std::is_same<double, CallableParameterType<LambdaType, 1>>::value, "");
   static_assert(std::is_same<const std::string&, CallableParameterType<LambdaType, 2>>::value, "");
   static_assert(std::is_same<const int&, CallableReturnType<LambdaType>>::value, "");
}
