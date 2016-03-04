#ifndef _THREAD_H_
#define _THREAD_H_

#include "visitor_ptr.h"
#include "matchObj.h"
#include "edgeManager.h"
#include <memory>
#include <vector>
#include <stack>

namespace rgx {

class _NFA_Node;

class _thread {
public:
    _thread(const visitor_ptr<_NFA_Node>&, unsigned int, unsigned int = 0);

    unsigned int _sp;                                  //字符串指针SP
    visitor_ptr<_NFA_Node>  _nodePtr;                  //nfa的节点指针，即PC
    unsigned int            _edgeIndex;                //对应节点的edge指针
    std::stack<std::pair<visitor_ptr<_NFA_Node>, unsigned int>>     _loopTimes;    //记录路径上各个循环的重复次数

    int match(const std::u16string &, std::stack<_thread>&, const std::unique_ptr<matchObj>&, const std::shared_ptr<_edgeManager>&);
    //返回0表示匹配成功, 其他值表示匹配失败
};


}
#endif

