#ifndef _STRING_TOOLS_
#define _STRING_TOOLS_

#include <string>
#include <memory>
#include <array>
#include <iostream>
#include <iomanip>


namespace rgx {

//the following are UBUNTU/LINUX ONLY terminal color codes.
extern const char* RESET         ;
extern const char* BLACK         ; /* Black */
extern const char* RED           ; /* Red */
extern const char* GREEN         ; /* Green */
extern const char* YELLOW        ; /* Yellow */
extern const char* BLUE          ; /* Blue */
extern const char* MAGENTA       ; /* Magenta */
extern const char* CYAN          ; /* Cyan */
extern const char* WHITE         ; /* White */
extern const char* BOLDBLACK     ; /* Bold Black */
extern const char* BOLDRED       ; /* Bold Red */
extern const char* BOLDGREEN     ; /* Bold Green */
extern const char* BOLDYELLOW    ; /* Bold Yellow */
extern const char* BOLDBLUE      ; /* Bold Blue */
extern const char* BOLDMAGENTA   ; /* Bold Magenta */
extern const char* BOLDCYAN      ; /* Bold Cyan */
extern const char* BOLDWHITE     ; /* Bold White */



std::string int2string(int);                                    //int转string
std::string bool2string(bool);                                  //bool转string
std::string ucs2_to_string(const std::u16string&);              //将ucs2编码的u16string转换为utf8编码的string
std::u16string string_to_ucs2(const std::string&);              //将utf8编码的string转换为ucs2编码的u16string
bool isDigit(char16_t);                                         //判断是否是数字
bool isLetter(char16_t);                                        //判断是否为字母
bool isSpace(char16_t);                                         //判断是否为空白字符
bool isLittleEndien();                                          //判断机器大小端，在将u16string转string时需要用到



template <typename tStringType, typename tTraits = typename tStringType::traits_type>
void print_code_unit_sequence(tStringType str) {
    using char_type = typename tTraits::char_type;
    static_assert(std::is_same<char_type, char>::value || std::is_same<char_type, char16_t>::value || std::is_same<char_type, char32_t>::value, "error");
    using unsigned_char_type = typename std::make_unsigned<char_type>::type;
    using unsigned_int_type = typename std::make_unsigned<typename tTraits::int_type>::type;
    int w = std::is_same<char, char_type>::value ? 2 : std::is_same<char16_t, char_type>::value ? 4 : 8;
    for (auto c : str) {
        auto value = static_cast<unsigned_int_type>(static_cast<unsigned_char_type>(c));
        std::cout << "0x" << std::hex << std::uppercase << std::setw(w) << std::setfill('0') << value << ' ';
    }
}

}


#endif
