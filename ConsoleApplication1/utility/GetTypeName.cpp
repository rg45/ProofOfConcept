#include "stdafx.h"

#include <boost/regex.hpp>
#include <string>

namespace poc
{
namespace utility
{
namespace GetTypeName_detail
{
namespace
{

const boost::regex functionSignaturePattern(R"(^[^<>]*(?:<(.*)>)?[^\[\]]*(?:\[with T = (.*)\])?.*$)");

} // namespace

std::string extractTypeNameFromFunctionSignature(const std::string& functionSignature)
{
   boost::smatch match;
   return
      boost::regex_match(functionSignature, match, functionSignaturePattern) ?
      match[2].matched ? match[2] : match[1] :
      std::string();
}

} // namespace GetTypeName_detail
} // namespace utility
} // namespace poc
