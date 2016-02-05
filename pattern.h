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

typedef std::pair<std::shared_ptr<_NFA_Node>, std::shared_ptr<_NFA_Node>> _NFA;
typedef std::shared_ptr<_NFA>  _NFA_ptr;
typedef std::vector<std::vector<unsigned int>> _DFA;
typedef std::shared_ptr<_DFA> _DFA_ptr;


class _pattern {
    friend class _NFA_Node;
public:
    _pattern(const _ast&);
    virtual std::shared_ptr<matchObj> match(const std::u16string&) = 0;
    virtual std::shared_ptr<matchObj> search(const std::u16string&) = 0;
    virtual std::shared_ptr<std::vector<matchObj>> findall(const std::u16string&) = 0;
    void traversal();                                      // 广度优先对NFA进行遍历, 用于debug- -
    void err();
    void err(const std::string&);

protected:
    std::shared_ptr<_edgeManager> _edgeMgr;
    _NFA_ptr _NFAptr;
    unsigned int _NFA_nodeCount;         //计算NFA节点的数量，同时为每个NFA节点分配一个唯一的ID, 加速后续算法

private:
    void epsilonCut();               //除去不必要的epsilon边
};

class _dfa_pattern : public _pattern {
public:
    _dfa_pattern (const _ast&);
    virtual std::shared_ptr<matchObj> match(const std::u16string&);
    virtual std::shared_ptr<matchObj> search(const std::u16string&);
    virtual std::shared_ptr<std::vector<matchObj>> findall(const std::u16string&);
    _DFA_ptr _DFAptr;
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
};

}

#endif
