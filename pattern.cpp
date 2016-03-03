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

rgx::_pattern::_pattern(_ast& ast) : _edgeMgr(ast._edgeMgr), _nameMap(std::move(ast._nameMap)) {
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
        cout << " node : " << dq.front().index();
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
            mergeClosure(vptr);
        }
    } 
    for (unsigned int i = 0; i < _objPool.capacity(); ++i) {
        auto vptr = _objPool.get_visitor(i);
        if (!vptr->_effective) {
            _objPool.release(vptr);
        } else {
            vptr->deleteEpsilonEdge();
        }
    }
}

void rgx::_pattern::mergeClosure(visitor_ptr<_NFA_Node>& keyNode) {

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
    vector<bool> unVisited(_objPool.capacity(), true);
    unVisited[keyNode.index()] = false;

    //寻找空闭包中的所有节点, 并复制非epsilon边到起点
    vector<visitor_ptr<_NFA_Node>> candidate[2];
    unsigned int current = 0, next = 1;
    candidate[current].push_back(keyNode);
    while (!candidate[current].empty()) {
        for (auto &ptr : candidate[current]) {
            for (unsigned int i = 0; i < ptr->edges.size(); ++i) {
                if (ptr->edges[i]->isEpsilonEdge() && unVisited[ptr->edges[i]->_toNode.index()]) {
                    unVisited[ptr->edges[i]->_toNode.index()] = false;
                    for (auto &edge : ptr->edges[i]->_toNode->edges) {
                        if (!edge->isEpsilonEdge()) {
                            keyNode->edges.push_back(unique_ptr<_NFA_Edge>(edge->makeCopy()));
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


std::unique_ptr<matchObj> _dfa_pattern::match(const std::u16string&) {
    return nullptr;
}

std::shared_ptr<matchObj> _dfa_pattern::search(const std::u16string&) {
    return make_shared<matchObj>();
}

std::shared_ptr<std::vector<matchObj>> _dfa_pattern::findall(const std::u16string&) {
    return make_shared<vector<matchObj>>();
}


/*===================================NFA_PATTERN===================================*/

_nfa_pattern::_nfa_pattern(_ast& ast) : _pattern(ast) {
    if (ast._build_type != _ast::build_to_nfa) {
        err("ast与pattern类型不一致，应为_build_to_nfa");
    }

}

std::unique_ptr<matchObj> _nfa_pattern::match(const std::u16string&) {
    unsigned int _pos = 0;
    ++_pos;
    return nullptr;
}

std::shared_ptr<matchObj> _nfa_pattern::search(const std::u16string&) {
    return make_shared<matchObj>();
}

std::shared_ptr<std::vector<matchObj>> _nfa_pattern::findall(const std::u16string&) {
    return make_shared<vector<matchObj>>();
}

std::unique_ptr<matchObj> rgx::_nfa_pattern::backtrackingVM() {
    stack<_thread> threadstack;    
    threadstack.push(_thread());
    unique_ptr<matchObj> storgePtr;
    while (!threadstack.empty()) {
        threadstack.top();
        threadstack.pop();
    }
    return nullptr; 
}

