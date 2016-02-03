#ifndef _PATTERN_H_
#define _PATTERN_H_

#include <memory>
#include <string>
#include "matchObj.h"
#include "edgeManager.h"

namespace rgx{

/*
 *   一个pattern包括一个finite automaton, 以及一组对应的匹配接口，
 *   这组接口接受一个匹配字符串，并返回一个matchObj来表示匹配结果，
 *   在pattern的建立过程中,利用ast节点中生成边的虚函数来生成自动机的节点,
 *   将生成自动机的任务下发到ast中的节点，可以很好的屏蔽
 *   不同的节点类型带来的差异，反而利用多态的特性使得可以正确的
 *   生成自动机，代码也会更清晰（个人向ˇˍˇ） 
 * 
 */


class _NFA_Node;
class _ast;

class _pattern {
public:
    virtual std::shared_ptr<matchObj> match(const std::u16string&) = 0;
    virtual std::shared_ptr<matchObj> search(const std::u16string&) = 0;
    virtual std::shared_ptr<std::vector<matchObj>> findall(const std::u16string&) = 0;
    virtual void traversal();
    void err();
    void err(const std::string&);
};

class _dfa_pattern : public _pattern {
public:
    _dfa_pattern (const _ast&);
    virtual std::shared_ptr<matchObj> match(const std::u16string&);
    virtual std::shared_ptr<matchObj> search(const std::u16string&);
    virtual std::shared_ptr<std::vector<matchObj>> findall(const std::u16string&);
    virtual void traversal();
private:
    std::shared_ptr<_edgeManager> _edgeMgr;
};

class _nfa_pattern : public _pattern {
public:
    _nfa_pattern (const _ast&); 
    std::shared_ptr<_NFA_Node> startNode;
    std::shared_ptr<_NFA_Node> finishNode;
    void generateNFA();
    virtual std::shared_ptr<matchObj> match(const std::u16string&);
    virtual std::shared_ptr<matchObj> search(const std::u16string&);
    virtual std::shared_ptr<std::vector<matchObj>> findall(const std::u16string&);
    virtual void traversal();
private:
    std::shared_ptr<_edgeManager> _edgeMgr;
};

}

#endif
