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
    _thread();
    _thread(const visitor_ptr<_NFA_Node>&, const std::shared_ptr<_edgeManager>&, unsigned int);
    unsigned int _sp;                               //字符串指针SP
    visitor_ptr<_NFA_Node>  _pc;                    //nfa的节点指针，即PC
    
    int match(std::stack<_thread>&, const std::unique_ptr<matchObj>&);
    std::shared_ptr<_edgeManager> _edgeMgr;
};



}
#endif
