#include <functional>
#include <utility>

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
