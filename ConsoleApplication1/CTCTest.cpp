#include "stdafx.h"

#include <iostream>
#include <type_traits>
#include <limits>

namespace
{
#define TRACE(exp) (std::cout << #exp " = " << (exp) << std::endl)

#define IS_COMPLETE(T) IsComplete<T, __COUNTER__>::value

#define GET_CTC(ctc) GetCTC<ctc, __COUNTER__>::value

#define INCREMENT_CTC(ctc) \
template <> \
struct ctc<GET_CTC(ctc) + 1> {};

#define CREATE_CTC(name) \
  template <CTCBase> struct name; \
  template <> struct name<0> {};

template <typename, int, typename = size_t>
struct IsComplete : std::false_type {};

template <typename T, int C>
struct IsComplete<T, C, decltype(sizeof(T))> : std::true_type {};

using CTCBase = int;

template <template <CTCBase> class CTC, int C, CTCBase BEGIN = 0,
   CTCBase COUNT = std::numeric_limits<CTCBase>::max(), typename = void>
struct GetCTC;

template <template <CTCBase> class CTC, int C, CTCBase BEGIN, CTCBase COUNT>
struct GetCTC<CTC, C, BEGIN, COUNT,
   typename std::enable_if<(COUNT >= 2) && IsComplete<CTC<BEGIN + COUNT / 2>, C>::value>::type>
   : GetCTC<CTC, C, BEGIN + COUNT/2, COUNT - COUNT/2> {};

template <template <CTCBase> class CTC, int C, CTCBase BEGIN, CTCBase COUNT>
struct GetCTC<CTC, C, BEGIN, COUNT,
   typename std::enable_if<(COUNT >= 2) && !IsComplete<CTC<BEGIN + COUNT / 2>, C>::value>::type>
   : GetCTC<CTC, C, BEGIN, COUNT / 2> {};

template <template <CTCBase> class CTC, int C, CTCBase BEGIN, CTCBase COUNT>
struct GetCTC<CTC, C, BEGIN, COUNT, typename std::enable_if<COUNT == 1>::type>
   : std::integral_constant<CTCBase, BEGIN> {};

class A;
constexpr auto c1 = IS_COMPLETE(A);
class A {};
constexpr auto c2 = IS_COMPLETE(A);

CREATE_CTC(ctc1)
constexpr auto d1 = GET_CTC(ctc1);
INCREMENT_CTC(ctc1)
constexpr auto d2 = GET_CTC(ctc1);
INCREMENT_CTC(ctc1)
constexpr auto d3 = GET_CTC(ctc1);
INCREMENT_CTC(ctc1)
constexpr auto d4 = GET_CTC(ctc1);

} // namespace

void CTCTest()
{
   std::cout << std::boolalpha;

   TRACE(IS_COMPLETE(int));
   TRACE(IS_COMPLETE(void));
   TRACE(IS_COMPLETE(class X));
   class X {};
   TRACE(IS_COMPLETE(class X));
   TRACE(c1);
   TRACE(c2);
   TRACE(d1);
   TRACE(d2);
   TRACE(d3);
   TRACE(d4);

}
