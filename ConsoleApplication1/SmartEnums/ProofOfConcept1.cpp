#include "stdafx.h"

#include <boost/preprocessor.hpp>
#include <iostream>
#include <utility>

namespace SmartEnums
{
namespace ProofOfContcept1
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

template <typename T>
constexpr auto IsCompleteV = IsComplete<T>::value;

namespace SmartEnums
{

template <typename T, typename U = unsigned>
class $martEnumBase$
{
protected:
   enum class $Entry$ : U;
public:

   $martEnumBase$() = default;
   $martEnumBase$($Entry$ v) : m(v) { }
   explicit $martEnumBase$(U v) : m(v) { }

   operator $Entry$() const { return m; }
   explicit operator U() const { return U(m); }

private:
   $Entry$ m = $Entry$();

   friend constexpr auto operator | ($Entry$ lhs, $Entry$ rhs) { return $Entry$((U(lhs) | U(rhs))); }
   friend constexpr auto operator & ($Entry$ lhs, $Entry$ rhs) { return $Entry$((U(lhs) & U(rhs))); }
   friend auto& operator << (std::ostream& output, $Entry$ v) { return output << "[Smart Enum]: " << U(v); }
};

template <typename, typename = void>
struct BuiltinEnumTraits;

template <typename T>
struct BuiltinEnumTraits<T, EnableIf<std::is_enum<T>::value && IsComplete<T>::value>>
{
   using UnderlyingType = typename std::underlying_type<T>::type;
};

template <typename T>
using IsBuiltinEnum = IsComplete<BuiltinEnumTraits<T>>;

template <typename T>
using UnderlyingType = typename BuiltinEnumTraits<T>::UnderlyingType;

template <typename T>
constexpr EnableIf<IsBuiltinEnum<T>::value, T> operator | (T lhs, T rhs)
{
  return T(UnderlyingType<T>(lhs) | UnderlyingType<T>(rhs));
}

} // namespace SmartEnums

namespace ns
{

class TestEnum : public SmartEnums::$martEnumBase$<TestEnum>
{
public:

   static constexpr auto Zero = $Entry$(0);
   static constexpr auto One = $Entry$(1);
   static constexpr auto Two = $Entry$(2);
   static constexpr auto Three = One | Two;

   using $martEnumBase$::$martEnumBase$;
};

template <typename T, typename U>
auto& operator << (std::ostream& output, const std::pair<T, U>& p)
{
   return output << "(" << p.first << ", " << p.second << ")";
}

enum class TestEnum2 : unsigned;
enum class TestEnum2 : SmartEnums::UnderlyingType<TestEnum2>
{
   one = 1,
   two,
   three = one | two,
   $SmartEnum$ = 0
};
using SmartEnums::operator|;

} // namespace ns

void main()
{
   const ns::TestEnum2 x = ns::TestEnum2::one | ns::TestEnum2::two;

   using ns::TestEnum;

   std::cout << std::boolalpha;

   std::cout << TestEnum::Zero << std::endl;
   std::cout << TestEnum::One << std::endl;
   std::cout << TestEnum::Two << std::endl;
   std::cout << TestEnum::Three << std::endl;
   std::cout << "---" << std::endl;

   const TestEnum v = TestEnum::Three;
   std::cout << v << std::endl;
   std::cout << (v == TestEnum::Two) << std::endl;
   std::cout << (v == TestEnum::Three) << std::endl;

   switch (v & TestEnum::One)
   {
   case TestEnum::One:
      std::cout << "Matched!" << std::endl;
      break;
   };

   const std::pair<TestEnum, std::string> p { TestEnum::Three, "Three" };
   std::cout << p << std::endl;
   std::cout << "---" << std::endl;

#define MAKE_ENUM_ENTRY(t) BOOST_PP_TUPLE_ELEM(0, t) \
   BOOST_PP_IF(BOOST_PP_EQUAL(2, BOOST_PP_TUPLE_SIZE(t)), = BOOST_PP_TUPLE_ELEM(1, t),)

#define TEST_SEQ_FOR_EACH(r, data, i, elem) \
   std::cout << "["#i"] " BOOST_PP_STRINGIZE(MAKE_ENUM_ENTRY(elem)) "," << std::endl;

   BOOST_PP_SEQ_FOR_EACH_I(TEST_SEQ_FOR_EACH, _, BOOST_PP_VARIADIC_SEQ_TO_SEQ(
      (one, 1)
      (two)
      (three, one | two)
   ));

}

} // namespace ProofOfContcept1
} // namespace SmartEnums
