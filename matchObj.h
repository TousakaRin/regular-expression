#ifndef _MATCH_OBJ_H_
#define _MATCH_OBJ_H_

#include <string>
#include <vector>
#include <map>

namespace rgx {

class _captureStartEdge;
class _captureEndEdge;
class _referenceEdge;

class matchObj {
    friend _captureStartEdge;
    friend _captureEndEdge;
    friend _referenceEdge;

public:
    matchObj() : _pos(0), _endpos(0), _lastindex(0), _lastCaptureIndex(0) {}
    std::vector<std::wstring>  group(unsigned int index);
    //返回编号为index的group，index为0时，表示匹配到的字符串
    

    std::vector<std::vector<std::wstring>> groups();
    //返回所有的group

    std::pair<unsigned int, unsigned int> span(unsigned int index);
    //返回匹配到得字符串在正文中的位置
    
    std::vector<std::wstring> group(const std::wstring& name);
    //返回别名为name的捕获列表
    
    void clear(); 

    std::u16string string() const;        //input
    std::u16string re() const;            //正则
    unsigned int pos() const;             //起点
    unsigned int endpos() const;          // 终点
    int lastindex() const;                
    int lastgroup() const;

private:
    unsigned int _pos;
    unsigned int _endpos;
    unsigned int _lastindex;
    unsigned _lastCaptureIndex;
    std::vector<std::pair<unsigned int, unsigned int>> _capVector;
};



}
#endif

