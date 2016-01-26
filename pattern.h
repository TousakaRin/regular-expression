#ifndef _PATTERN_H_
#define _PATTERN_H_

#include <memory>
#include "matchObj.h"
#include "ast.h"
#include "fa_node.h"
#include <string>

namespace rgx{

/*
 *   pattern表示一个DFA或者NFA，在建立过程中
 *   利用ast节点中生成边的虚函数来生成自动机的节点,
 *   将生成自动机的任务下发到ast中的节点，可以很好的屏蔽
 *   不同的节点类型带来的差异，反而利用多态的特性使得可以正确的
 *   生成自动机，代码也会更清晰（个人向(ˇˍˇ） 
 * 
 */

class pattern {
public:
    virtual std::shared_ptr<matchObj> match(const std::u16string&) = 0;
    virtual std::shared_ptr<matchObj> search(const std::u16string&) = 0;
    virtual std::shared_ptr<std::vector<matchObj>> findall(const std::u16string&) = 0;
};

class dfa_pattern : public pattern {
public:
    dfa_pattern (const _ast&);
    virtual std::shared_ptr<matchObj> match(const std::u16string&);
    virtual std::shared_ptr<matchObj> search(const std::u16string&);
    virtual std::shared_ptr<std::vector<matchObj>> findall(const std::u16string&);
};

class nfa_pattern : public pattern {
public:
    nfa_pattern (const _ast&); 
    std::shared_ptr<_NFA_Node> startNode;
    virtual std::shared_ptr<matchObj> match(const std::u16string&);
    virtual std::shared_ptr<matchObj> search(const std::u16string&);
    virtual std::shared_ptr<std::vector<matchObj>> findall(const std::u16string&);
};

}

#endif
