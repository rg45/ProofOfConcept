#include "stdafx.h"

#include <boost/preprocessor.hpp>

namespace
{
namespace CompileTimeStrings
{

#define STATIC_ASSERT(...) static_assert(__VA_ARGS__, #__VA_ARGS__)

class str_const
{
public:
   template<std::size_t N>
   constexpr str_const(const char(&a)[N]) : p_(a), sz_(N - 1) {}

   constexpr char operator[](std::size_t n) const { return n < sz_ ? p_[n] : throw std::out_of_range(""); }

   constexpr std::size_t size() const { return sz_; }

private:
   const char* const p_;
   const std::size_t sz_;
};

template <char...C>
struct CTString
{
   static const char buf[];
};

template <char...C>
const char CTString<C...>::buf[] = { C... };

template <size_t S>
using TestSize = std::integral_constant<size_t, S>;

void run()
{
   std::cout << TestSize<sizeof("Hello, World!!!")>::value << std::endl;

}

} // namespace CompileTimeStrings
} // namespace

void CompileTimeStrings()
{
   using namespace CompileTimeStrings;
   run();
}