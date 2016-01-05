#ifndef _STRING_TOOLS_
#define _STRING_TOOLS_

#include <string>

namespace rgx {

std::string int2string(int);
std::string bool2string(bool);
std::string w2c(const wchar_t * pw);
std::wstring c2w(const char *pc);
std::string wstring_to_utf8 (const std::wstring& str);
std::wstring utf8_to_wstring (const std::string& str);
}
#endif
