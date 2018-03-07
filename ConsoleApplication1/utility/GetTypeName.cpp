#include "stdafx.h"

#include <boost/regex.hpp>
#include <string>

namespace poc
{
namespace utility
{
namespace GetTypeName_detail
{

std::string extractTypeNameFromFunctionSignature(const std::string& signature)
{
   static const boost::regex pattern("^[^<]*<(.*)>[^>]*$");
   boost::smatch match;
   if (regex_match(signature, match, pattern))
      return match[1];
   
   throw std::runtime_error("Invalid signature: " + signature);
}

} // namespace GetTypeName_detail
} // namespace utility
} // namespace poc
