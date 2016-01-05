#include "stringTools.h"
#include <codecvt>
#include <algorithm>
#include <locale>

using namespace rgx;
using namespace std;

string rgx::int2string(int i) {
    string s;
    if (i == 0) {
        return "0";
    }
    if (i < 0) {
        i = -i;
        s.push_back('-');
    }
    string tmp;
    while(i != 0) {
        tmp.push_back(i % 10);
        i /= 10;
    }
    reverse(tmp.begin(), tmp.end());
    s += tmp;
    return s;
} 

string rgx::bool2string(bool b) {
    if (b) {
        return "true";
    } else {
        return "false";
    }
}



// convert UTF-8 string to wstring
std::wstring rgx::utf8_to_wstring (const std::string& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.from_bytes(str);
}

// convert wstring to UTF-8 string
std::string rgx::wstring_to_utf8 (const std::wstring& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.to_bytes(str);
}
