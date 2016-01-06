#ifndef _STRING_TOOLS_
#define _STRING_TOOLS_

#include <string>


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

std::string int2string(int);
std::string bool2string(bool);
std::string w2c(const wchar_t * pw);
std::wstring c2w(const char *pc);
std::string wstring_to_utf8 (const std::wstring& str);
std::wstring utf8_to_wstring (const std::string& str);
}
#endif
