#ifndef _PATTERN_H_
#define _PATTERN_H_

#include <memory>
#include <iostream>
#include <map>
#include <string>
#include "matchObj.h"
#include "edgeManager.h"
#include "nfaNode.h"
#include "visitor_ptr.h"
#include "objectPool.h"

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

typedef std::vector<std::vector<unsigned int>> _DFA;
typedef std::shared_ptr<_DFA> _DFA_ptr;


class _pattern {
    friend class _NFA_Node;
public:
    _pattern(_ast&);
    virtual std::unique_ptr<matchObj> match(const std::u16string&) = 0;
    virtual std::shared_ptr<matchObj> search(const std::u16string&) = 0;
    virtual std::shared_ptr<std::vector<matchObj>> findall(const std::u16string&) = 0;
    void traversal();                                      // 广度优先对NFA进行遍历, 用于debug- -
    void err();
    void err(const std::string&);
    _objectPool<_NFA_Node>& getObjPool() { return _objPool; }
    void sequenceTraversal();

protected:
    std::shared_ptr<_edgeManager> _edgeMgr;
    std::unique_ptr<std::map<std::u16string, unsigned int>> _nameMap;                               //具名捕获-->捕获index的转换
    _NFA_Ptr _NFAptr;

private:
    void epsilonCut();                                  //除去不必要的epsilon边
    _objectPool<_NFA_Node>  _objPool;
    void mergeClosure(visitor_ptr<_NFA_Node>&);         //将闭包中的边拷贝到关键节点中,用于epsilonCut() 

};

class _dfa_pattern : public _pattern {
public:
    _dfa_pattern (_ast&);
    virtual std::unique_ptr<matchObj> match(const std::u16string&);
    virtual std::shared_ptr<matchObj> search(const std::u16string&);
    virtual std::shared_ptr<std::vector<matchObj>> findall(const std::u16string&);
};

class _nfa_pattern : public _pattern {
public:
    _nfa_pattern (_ast&); 
    void generateNFA();
    virtual std::unique_ptr<matchObj> match(const std::u16string&);
    virtual std::shared_ptr<matchObj> search(const std::u16string&);
    virtual std::shared_ptr<std::vector<matchObj>> findall(const std::u16string&);

private:
    std::unique_ptr<matchObj> backtrackingVM();
};

}

#endif

