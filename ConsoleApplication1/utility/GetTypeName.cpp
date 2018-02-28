#include "stdafx.h"

#include <string>

namespace poc
{
namespace utility
{
namespace GetTypeName_detail
{

std::string extractTypeNameFromFunctionSignature(const std::string& functionSignature)
{
   const auto openBracketPos = functionSignature.find('<');
   const auto closeBracketPos = functionSignature.rfind('>');
   return openBracketPos != std::string::npos && closeBracketPos != std::string::npos ?
      functionSignature.substr(openBracketPos + 1, closeBracketPos - openBracketPos - 1) :
      std::string();
}

} // namespace GetTypeName_detail
} // namespace utility
} // namespace poc
