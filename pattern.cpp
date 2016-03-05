#include <stack>
#include <set>
#include <deque>
#include <unordered_set>
#include "pattern.h"
#include "cstdlib"
#include "ast.h"
#include "astnode.h"
#include "nfaEdge.h"
#include "nfaNode.h"
#include "thread.h"

using namespace std;
using namespace rgx;


/*=================================PATTERN=========================================*/

rgx::_pattern::_pattern(_ast& ast) : _edgeMgr(ast._edgeMgr), _nameMap(std::move(ast._nameMap)), _captureIndex(ast._captureIndex + 1) {
    cout << "_CaptureInedx " << _captureIndex << endl;
    //使用非递归方法后序遍历ast来生成NFA
    stack<visitor_ptr<_astNode>> s;  
    auto p = ast._root;  
    do {  
        while(p) {  
            s.push(p);  
            p = p->_left;  
        }  
        while(!s.empty() && (!s.top()->_right || s.top()->_right->_NFAptr))  {  
            s.top()->generateNFA(*this);
            s.pop();  
        }            
        if(!s.empty()) {  
            p = s.top()->_right;  
        }  
    } while(!s.empty());  
    _NFAptr.reset(ast._root->_NFAptr.release());
    //将开始节点设为有效节点，用于空边消除算法
    _NFAptr->first->setEffective();
//    sequenceTraversal();
    epsilonCut();
//    sequenceTraversal();
}

void rgx::_pattern::err() {
    exit(-1);
}

void rgx::_pattern::err(const string& msg) {
    cout << "\n----------------------------------------------------------\n"
        << msg 
        << "\n----------------------------------------------------------\n";
    err();
}

void rgx::_pattern::sequenceTraversal() {
    cout << "----------------------sequenceTraversal-----------------------" << 
        "\n-------------------------------------------------------------------" << endl;
    for (unsigned int i = 0; i < _objPool.capacity(); ++i) {
        if (!_objPool._pool[i]) {
            cout << i << " null" << endl;
        } else {
            cout << i << endl;
            for (auto &edge : _objPool._pool[i]->edges) {
                cout << edge->toString() << endl;
            }
        }
    }    
}

void rgx::_pattern::traversal() {
    deque<visitor_ptr<_NFA_Node>> dq;
    set<_NFA_Node*> s;
    dq.push_back(_NFAptr->first);
    while(!dq.empty()) {
        s.insert(dq.front().get());
//        cout << " node : " << dq.front().index();
        for (unsigned int i = 0; i < dq.front()->edges.size(); ++i) {
            auto edge = dq.front()->edges[i].get();
            cout << edge->toString() << endl;
            if (s.find(edge->_toNode.get()) == s.end()) {
                dq.push_back(edge->_toNode);
                s.insert(edge->_toNode.get());
            }
        }
        dq.pop_front();
    }
    cout << "\n--------------------traversal completed---------------------------" << endl;
}

void rgx::_pattern::epsilonCut() {
    vector<visitor_ptr<_NFA_Node>> effectiveNodes;
    for (unsigned int i = 0; i < _objPool.capacity(); ++i) {
        auto vptr = _objPool.get_visitor(i);
        if (vptr->_effective) {
            mergeClosure(i);
        }
    } 
    for (unsigned int i = 0; i < _objPool.capacity(); ++i) {
        auto vptr = _objPool.get_visitor(i);
        if (!vptr->_effective) {
            _objPool.release(i);
        } else {
            vptr->deleteEpsilonEdge();
        }
    }
}

unique_ptr<matchObj> rgx::_pattern::match(const u16string&s, unsigned int startPosition) {
    return _match(s, startPosition);
}


void rgx::_pattern::mergeClosure(unsigned int keyNode) {

    /*
     * /将vptr指向的节点的  空闭包中的所有节点的边
     * ＊复制＊ 到vptr中, 为什么是复制而不是移动呢，因为可能要复制到
     * 很多个地方啊
     * 
     * 由于构造NFA时，若增加非epsilon边，必然会新建节点
     * 所以不存在同一个闭包中的两个节点的非epsilon边指向同一个节点的情况
     * 故这里只要找到闭包中的effective节点之外的所有非epsilon边然后
     * 复制过去就可以了
     *
     */
    
    //标记起点, 起点的边无需再复制
    set<visitor_ptr<_NFA_Node>> visited;
    visited.insert(_objPool.get_visitor(keyNode));

    //寻找空闭包中的所有节点, 并复制非epsilon边到起点
    vector<visitor_ptr<_NFA_Node>> candidate[2];
    unsigned int current = 0, next = 1;
    candidate[current].push_back(_objPool.get_visitor(keyNode));
    while (!candidate[current].empty()) {
        for (auto &ptr : candidate[current]) {
            for (unsigned int i = 0; i < ptr->edges.size(); ++i) {
                if (ptr->edges[i]->isEpsilonEdge() && visited.find(ptr->edges[i]->_toNode) != visited.end()) {
                    visited.insert(ptr->edges[i]->_toNode);
                    for (auto &edge : ptr->edges[i]->_toNode->edges) {
                        if (!edge->isEpsilonEdge()) {
                            _objPool.get_visitor(keyNode)->edges.push_back(unique_ptr<_NFA_Edge>(edge->makeCopy()));
                        }
                    }
                    candidate[next].push_back(ptr->edges[i]->_toNode);
                } 
            }
        }
        candidate[current].clear();
        swap(current, next);
    }
}

/*===================================DFA_PATTERN===================================*/

_dfa_pattern::_dfa_pattern(_ast& ast) : _pattern(ast)/*, _DFAptr(make_shared<_DFA>())*/ {
    //使用子集构造法建立DFA
  /*  unsigned int statsCount = _edgeMgr->statsCount();
    statsCount = 0;*/
}


std::unique_ptr<matchObj> rgx::_dfa_pattern::_match(const std::u16string&, unsigned int) {
    return nullptr;
}

std::unique_ptr<matchObj> rgx::_dfa_pattern::_search(const std::u16string&) {
    return nullptr;
}

std::unique_ptr<std::vector<matchObj>> rgx::_dfa_pattern::_findall(const std::u16string&) {
    return nullptr;
}


/*===================================NFA_PATTERN===================================*/

_nfa_pattern::_nfa_pattern(_ast& ast) : _pattern(ast) {
    if (ast._build_type != _ast::build_to_nfa) {
        err("ast与pattern类型不一致，应为_build_to_nfa");
    }

}

std::unique_ptr<matchObj> _nfa_pattern::_match(const u16string& input, unsigned int startPosition) {
    return backtrackingVM(input, startPosition);
}

std::unique_ptr<matchObj> _nfa_pattern::_search(const u16string&) {
    return nullptr;
}

std::unique_ptr<std::vector<matchObj>> _nfa_pattern::_findall(const u16string&) {
    return nullptr;
}

std::unique_ptr<matchObj> rgx::_nfa_pattern::backtrackingVM(const u16string &input, unsigned int startPosition) {
    stack<_thread> threadstack;    
    threadstack.push(_thread(_NFAptr->first, _captureIndex, startPosition, 0));
    while (!threadstack.empty()) {
        auto cthread = threadstack.top();
        threadstack.pop();
        if (cthread.match(input, threadstack, _edgeMgr) == 0) {
//            cthread._capture->justToTest("abcaabc");
            return std::move(cthread._capture);
        } 
    }
    return nullptr;
}

