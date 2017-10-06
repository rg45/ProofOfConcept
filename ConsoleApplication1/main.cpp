#include "stdafx.h"

#include <boost/preprocessor.hpp>
#include <utility>

template <typename T, typename = decltype(sizeof(int))>
struct IsComplete : std::false_type { };

template <typename T>
struct IsComplete<T, decltype(sizeof(T))> : std::true_type { };

template <typename T>
constexpr auto IsCompleteV = IsComplete<T>::value;

namespace SmartEnums
{

template <typename T, typename U = unsigned>
class SmartEnumBase
{
public:
   using UnderlyingType = U;
   enum class Entry : UnderlyingType { };

   SmartEnumBase() = default;
   SmartEnumBase(Entry v) : m(v) { }
   explicit SmartEnumBase(UnderlyingType v) : m(Entry(v)) { }

   operator Entry() const { return m; }

#pragma warning(push)
#pragma warning(disable : 4800)
   explicit operator UnderlyingType() const { return UnderlyingType(m); }
#pragma warning(pop)

private:
   Entry m = Entry();
};

//template <typename> struct SmartEnumTraits;
//template <typename T, typename U>
//struct SmartEnumTraits<typename SmartEnumBase<T, U>::Entry>
//{
//};

template <typename T, typename U>
std::ostream& operator<<(std::ostream& output, SmartEnumBase<T, U> v)
{
   return output << "TestEnum: " << SmartEnumBase<T, U>::UnderlyingType(v);
}

} // namespace SmartEnums

namespace ns
{

class TestEnum : public SmartEnums::SmartEnumBase<TestEnum>
{
public:
   static constexpr Entry e1 = Entry(IsCompleteV<int>);
   static constexpr Entry e2 = Entry(IsCompleteV<void>);
   static constexpr Entry e3 = Entry(IsCompleteV<void*>);
   static constexpr Entry e4 = Entry(IsCompleteV<void()>);
   static constexpr Entry e5 = Entry(IsCompleteV<void(*)()>);
   static constexpr Entry e6 = Entry(IsCompleteV<struct X>);
   static constexpr Entry e7 = Entry(IsCompleteV<struct Y*>);
   static constexpr Entry e8 = Entry(IsCompleteV<struct Y&>);

   using SmartEnumBase::SmartEnumBase;
};

} // namespace ns

int main()
{
   using ns::TestEnum;

   std::cout << std::boolalpha;
   std::cout << IsComplete<int>::value << std::endl;
   std::cout << IsComplete<void>::value << std::endl;
   std::cout << IsComplete<void*>::value << std::endl;
   std::cout << IsComplete<void()>::value << std::endl;
   std::cout << IsComplete<void(*)()>::value << std::endl;
   std::cout << IsComplete<struct X>::value << std::endl;
   std::cout << IsComplete<struct Y*>::value << std::endl;
   std::cout << IsComplete<struct Y&>::value << std::endl;
   std::cout << "---" << std::endl;
   std::cout << IsCompleteV<int> << std::endl;
   std::cout << IsCompleteV<void> << std::endl;
   std::cout << IsCompleteV<void*> << std::endl;
   std::cout << IsCompleteV<void()> << std::endl;
   std::cout << IsCompleteV<void(*)()> << std::endl;
   std::cout << IsCompleteV<struct X> << std::endl;
   std::cout << IsCompleteV<struct Y*> << std::endl;
   std::cout << IsCompleteV<struct Y&> << std::endl;
   std::cout << "---" << std::endl;
   std::cout << IsCompleteV<TestEnum::Entry> << std::endl;
   std::cout << IsCompleteV<std::is_enum<TestEnum::Entry>> << std::endl;
   std::cout << "---" << std::endl;

//   std::cout << TestEnum::e1 << std::endl;
//   std::cout << TestEnum::e2 << std::endl;
//   std::cout << TestEnum::e3 << std::endl;
//   std::cout << TestEnum::e4 << std::endl;
//   std::cout << TestEnum::e5 << std::endl;
//   std::cout << TestEnum::e6 << std::endl;
//   std::cout << TestEnum::e7 << std::endl;
//   std::cout << TestEnum::e8 << std::endl;
//   std::cout << "---" << std::endl;

   const TestEnum v = TestEnum::e7;
   std::cout << (v == TestEnum::e7) << std::endl;
   std::cout << (v == TestEnum::e8) << std::endl;

   switch (v)
   {
   case TestEnum::e7:
      std::cout << "Matched!" << std::endl;
      break;
   };

}