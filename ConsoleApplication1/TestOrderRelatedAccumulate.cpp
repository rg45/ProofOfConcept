#include "stdafx.h"

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <tuple>

namespace
{

enum OrderRelation
{
   unknown_relation = 0, // default value
   filled_independently = 1,
   consecutive_orders = 2,
   spread_orders = 3,
   any_of_orders = 4,
   order_relation_size = 5
};

namespace OrderRelationTreeSupport
{

template <typename T>
using MakeVoid = void;

template <OrderRelation, typename, typename, typename = void>
struct ClassHasAccumulateMethod : std::false_type { };

template <OrderRelation orderRelation, typename T, typename...ArgsT>
struct ClassHasAccumulateMethod<orderRelation, T, std::tuple<ArgsT...>,
   MakeVoid<decltype(std::declval<T&>().template Accumulate<orderRelation>(std::declval<ArgsT>()...))>> : std::true_type { };

template <OrderRelation, typename, typename, typename = void>
struct HasAccumulateMethod_ : std::false_type { };

template <OrderRelation orderRelation, typename T, typename...ArgsT>
struct HasAccumulateMethod_<orderRelation, T, std::tuple<ArgsT...>,
   typename std::enable_if<std::is_class<T>::value>::type> :
   ClassHasAccumulateMethod<orderRelation, T, std::tuple<ArgsT...>> { };

template <OrderRelation orderRelation, typename T, typename...ArgsT>
using HasAccumulateMethod = HasAccumulateMethod_<orderRelation, T, std::tuple<ArgsT...>>;

} // namespace OrderRelationTreeSupport

using OrderRelationTreeSupport::HasAccumulateMethod;

template <OrderRelation orderRelation, typename T, typename, typename = void>
struct DefaultOrderRelationAccumulator;

template <OrderRelation orderRelation, typename T, typename ArgsT, typename = void>
struct OrderRelationAccumulator : DefaultOrderRelationAccumulator<orderRelation, T, ArgsT> { };

template <OrderRelation orderRelation, typename T, typename...ArgsT>
struct OrderRelationAccumulator<orderRelation, T, std::tuple<ArgsT...>,
   typename std::enable_if<HasAccumulateMethod<orderRelation, T, ArgsT...>::value>::type>
{
   decltype(auto) operator()(T& t, ArgsT&&...args) const
   {
      return t.template Accumulate<orderRelation>(std::forward<ArgsT>(args)...);
   }
};

template <OrderRelation orderRelation, typename T, typename...ArgsT>
void AccumulateDefault(T& t, ArgsT&&...args)
{
   return DefaultOrderRelationAccumulator<orderRelation, T, std::tuple<ArgsT...>>()(t, std::forward<ArgsT>(args)...);
}

template <OrderRelation orderRelation, typename T, typename...ArgsT>
decltype(auto) Accumulate(T& t, ArgsT&&...args)
{
   return OrderRelationAccumulator<orderRelation, T, std::tuple<ArgsT...>>()(t, std::forward<ArgsT>(args)...);
}

struct A
{
   A() = default;
   A(int) {}

   template <OrderRelation>
   A& Accumulate(const A&)
   {
      std::cout << "A& A::Accumulate<OrderRelation>(const A&)" << std::endl;
      return *this;
   }
};

struct B
{

};

template <OrderRelation orderRelation, typename...ArgsT>
struct OrderRelationAccumulator<orderRelation, std::string, std::tuple<ArgsT...>>
{
   std::string& operator()(std::string& accumulator, ArgsT&&...args) const
   {
      std::ostringstream pack;
      std::initializer_list<int>{(pack << args, 0)...};
      return accumulator += pack.str();
   }
};

} // namespace

void TestOrderRelatedAccumulate()
{
//    int i;
//    Accumulate<filled_independently>(i, 42);

   A a;
   Accumulate<filled_independently>(Accumulate<filled_independently>(a, 42), a);

   //    B b;
   //    Accumulate<filled_independently>(b, 42);

   std::cout << HasAccumulateMethod<filled_independently, A, const A&>::value << std::endl;

   std::string s = "Hello";
   std::cout << Accumulate<filled_independently>(s, ", World!!! ", 3.14) << std::endl;
   std::cout << s << std::endl;
}
