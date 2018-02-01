#pragma once

#include <type_traits>

namespace poc
{
namespace utility
{

template <bool C, typename T = void>
using EnableIf = typename std::enable_if<C, T>::type;

} // namespace utility
} // namespace poc

