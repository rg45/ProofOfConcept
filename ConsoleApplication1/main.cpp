#include "stdafx.h"

#include <iostream>
#include <utility>

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

} // namespace ns

int main()
{
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
}