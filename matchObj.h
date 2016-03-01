#ifndef _MATCH_OBJ_H_
#define _MATCH_OBJ_H_

#include <string>
#include <vector>

namespace rgx {


class matchObj {
public:
    matchObj() : string(), re(), pos(0), endpos(0), lastindex(0), lastgroup(0) {}
    std::vector<std::wstring>  group(unsigned int index);
    //返回编号为index的group，index为0时，表示匹配到的字符串
    

    std::vector<std::vector<std::wstring>> groups();
    //返回所有的group

    std::pair<unsigned int, unsigned int> span(unsigned int index);
    //返回匹配到得字符串在正文中的位置
    
    std::vector<std::wstring> group(const std::wstring& name);
    //返回别名为name的捕获列表
    
    const std::u16string string;
    const std::u16string re;
    const unsigned int pos;
    const unsigned int endpos;
    const int lastindex;
    const int lastgroup;
private:
    std::wstring::size_type start, end;
    std::wstring matchedString;
    std::vector<std::vector<std::wstring>> caught;
};



}
#endif

