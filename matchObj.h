#ifndef _MATCH_OBJ_H_
#define _MATCH_OBJ_H_

#include <string>
#include <memory>
#include <vector>
#include <map>

namespace rgx {

class _captureStartEdge;
class _captureEndEdge;
class _referenceEdge;
class _thread;
class _nfa_pattern;
class _dfa_pattern;

class matchObj {
    friend _captureStartEdge;
    friend _captureEndEdge;
    friend _referenceEdge;
    friend _thread;
    friend _nfa_pattern;
    friend _dfa_pattern;

public:
    matchObj(unsigned int maxCaptureSlot)
        : _pos(0), _endpos(0), _lastindex(0), _lastCaptureIndex(0), _capVector(maxCaptureSlot, std::pair<unsigned int, unsigned int>(0, 0)) {}

	matchObj(const matchObj& matchobj)
		: _pos(matchobj._pos), _endpos(matchobj._endpos), _lastindex(matchobj._lastCaptureIndex), _nameMap(nullptr), _capVector(matchobj._capVector) {
			//拷贝时将re，input，_nameMap 默认初始化，这几个字段的初始化仅仅由addReAndInput函数提供
		}

    std::u16string  group(unsigned int index);
    //返回编号为index的group，index为0时，表示匹配到的字符串
    

    std::vector<std::u16string> groups();
    //返回所有的group

    std::pair<unsigned int, unsigned int> span(unsigned int index);
    //返回匹配到得字符串在正文中的位置
    
    std::u16string group(const std::string& name);
    //返回别名为name的捕获列表
    

    std::u16string string() const;        //input
    std::u16string re() const;            //正则
    unsigned int pos() const;             //起点
    unsigned int endpos() const;          // 终点
    int lastindex() const;                
    int lastgroup() const;

    void justToTest(const std::u16string&);

private:
    std::u16string _re;
    std::u16string _input;
    unsigned int _pos;
    unsigned int _endpos;
    unsigned int _lastindex;
    unsigned _lastCaptureIndex;
    std::unique_ptr<std::map<std::u16string, unsigned int>> _nameMap;         //具名捕获-->捕获index的转换
    std::vector<std::pair<unsigned int, unsigned int>> _capVector;
    void addReAndInput(const std::u16string&re, const std::u16string&input, const std::unique_ptr<std::map<std::u16string, unsigned int>>&);
};



}
#endif

