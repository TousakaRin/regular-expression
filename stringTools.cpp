#include "stringTools.h"
#include <codecvt>
#include <algorithm>
#include <locale>
#include <iostream>

using namespace rgx;
using namespace std;

//the following are UBUNTU/LINUX ONLY terminal color codes.
const char* rgx::RESET         = "\033[0m";
const char* rgx::BLACK         = "\033[30m"; /* Black */
const char* rgx::RED           = "\033[31m"; /* Red */
const char* rgx::GREEN         = "\033[32m"; /* Green */
const char* rgx::YELLOW        = "\033[33m"; /* Yellow */
const char* rgx::BLUE          = "\033[34m"; /* Blue */
const char* rgx::MAGENTA       = "\033[35m"; /* Magenta */
const char* rgx::CYAN          = "\033[36m"; /* Cyan */
const char* rgx::WHITE         = "\033[37m"; /* White */
const char* rgx::BOLDBLACK     = "\033[1m\033[30m"; /* Bold Black */
const char* rgx::BOLDRED       = "\033[1m\033[31m"; /* Bold Red */
const char* rgx::BOLDGREEN     = "\033[1m\033[32m"; /* Bold Green */
const char* rgx::BOLDYELLOW    = "\033[1m\033[33m"; /* Bold Yellow */
const char* rgx::BOLDBLUE      = "\033[1m\033[34m"; /* Bold Blue */
const char* rgx::BOLDMAGENTA   = "\033[1m\033[35m"; /* Bold Magenta */
const char* rgx::BOLDCYAN      = "\033[1m\033[36m"; /* Bold Cyan */
const char* rgx::BOLDWHITE     = "\033[1m\033[37m"; /* Bold White */



bool rgx::isLittleEndien() {
    unsigned int i = 0x12345678;
    unsigned char *p = reinterpret_cast<unsigned char*>(&i);
    if (static_cast<unsigned int>(*p) == 0x12) {
        return false;
    } else {
        return true;
    }
}

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
        tmp.push_back(i % 10 + '0');
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


string rgx::ucs2_to_string(const std::u16string& str) {
    static wstring_convert<codecvt_utf8<char16_t>, char16_t> myconv;
    return myconv.to_bytes(str);
}

u16string rgx::string_to_ucs2(const std::string& str) {
    static wstring_convert<codecvt_utf8<char16_t, 0x10ffff, std::little_endian>, char16_t> myconv_little_endian;
    static wstring_convert<codecvt_utf8<char16_t>, char16_t> myconv_big_endian;
    if (isLittleEndien()) {
        return myconv_little_endian.from_bytes(str);
    } else {
        return myconv_big_endian.from_bytes(str);
    }
}

bool rgx::isDigit(char16_t c) {
    return c >= '0' && c <= '9';
}

bool rgx::isLetter(char16_t c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

