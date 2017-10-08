#include "stdafx.h"

#include <iostream>
#include <map>
#include <utility>

namespace
{
namespace SmartEnums2
{
namespace SmartEnums
{

template <bool cond, typename type = void>
using EnableIf = typename std::enable_if<cond, type>::type;

template <typename, typename type = void>
struct EnableIfDeclared_ : std::enable_if<true, type> { };

template <typename T, typename type = void>
using EnableIfDeclared = typename EnableIfDeclared_<T, type>::type;

template <typename T, typename = decltype(sizeof(int))>
struct IsComplete : std::false_type { };

template <typename T>
struct IsComplete<T, decltype(sizeof(T))> : std::true_type { };

template <typename, typename = void>
struct SmartEnumTraits;

template <typename T>
struct SmartEnumTraits<T, EnableIf<std::is_enum<T>::value && IsComplete<T>::value>>
{
   using UnderlyingType = typename std::underlying_type<T>::type;
};

template <typename T>
using IsSmartEnum = IsComplete<SmartEnumTraits<T>>;

template <typename T>
using UnderlyingType = typename SmartEnumTraits<T>::UnderlyingType;

template <typename, size_t>
struct EntryTraits;

template <typename T, size_t I, UnderlyingType<T> V>
struct EntryRegister
{
};

template <typename T, size_t I, UnderlyingType<T> value>
constexpr auto RegisterEntry()
{
   return value;
}

} // namespace SmartEnums

void run()
{
   enum class TestEnum
   {
      one = SmartEnums::RegisterEntry<TestEnum, 0, 1>(),
      two = SmartEnums::RegisterEntry<TestEnum, 1, 2>(),
   };

   std::cout << SmartEnums::UnderlyingType<TestEnum>(TestEnum::one) << std::endl;
   std::cout << SmartEnums::UnderlyingType<TestEnum>(TestEnum::two) << std::endl;
}

} // namespace SmartEnums2
} // namespace

void SmartEnums2()
{
   SmartEnums2::run();
}