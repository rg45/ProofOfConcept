#include <string>
#include <boost/current_function.hpp>

namespace poc
{
namespace utility
{
namespace GetTypeName_detail
{

std::string extractTypeNameFromFunctionSignature(const std::string&);

template <typename T>
class GetTypeNameImpl
{
public:

   GetTypeNameImpl() : m_typeName(extractTypeNameFromFunctionSignature(BOOST_CURRENT_FUNCTION)) { }

   std::string&& operator()() && { return std::move(m_typeName); }

private:
   std::string m_typeName;
};

} // namespace GetTypeName_detail

template <typename T>
std::string GetTypeName()
{
   return GetTypeName_detail::GetTypeNameImpl<T>()();
}

} // namespace utility
} // namespace poc
