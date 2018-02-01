#include <string>

namespace poc
{
namespace utility
{
namespace detail
{
template <typename T>
class GetTypeNameImpl
{
public:

   constexpr GetTypeNameImpl()
   : m_funcSig(__FUNCSIG__)
   , m_typenNameBegin(m_funcSig.find('<') + 1)
   , m_typeNameSize(m_funcSig.rfind('>') - m_typenNameBegin)
   {
   }

   constexpr std::string operator()() const { return m_funcSig.substr(m_typenNameBegin, m_typeNameSize); }

private:
   std::string m_funcSig;
   size_t m_typenNameBegin;
   size_t m_typeNameSize;
};

} // namespace detail

template <typename T>
constexpr std::string GetTypeName()
{
   return detail::GetTypeNameImpl<T>()();
}

} // namespace utility
} // namespace poc
