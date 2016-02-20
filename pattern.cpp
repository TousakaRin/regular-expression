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

using namespace std;
using namespace rgx;


/*=================================PATTERN=========================================*/

rgx::_pattern::_pattern(_ast& ast) : _edgeMgr(ast._edgeMgr)  {
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
    epsilonCut();
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

void rgx::_pattern::traversal() {
    deque<visitor_ptr<_NFA_Node>> dq;
    set<_NFA_Node*> s;
    dq.push_back(_NFAptr->first);
    while(!dq.empty()) {
        s.insert(dq.front().get());
    //    cout << " node : " << dq.front()->index();
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
}

/*===================================DFA_PATTERN===================================*/

_dfa_pattern::_dfa_pattern(_ast& ast) : _pattern(ast)/*, _DFAptr(make_shared<_DFA>())*/ {
    //使用子集构造法建立DFA
  /*  unsigned int statsCount = _edgeMgr->statsCount();
    statsCount = 0;*/
}


std::shared_ptr<matchObj> _dfa_pattern::match(const std::u16string&) {
    return make_shared<matchObj>();
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

std::shared_ptr<matchObj> _nfa_pattern::match(const std::u16string&) {
    return make_shared<matchObj>();
}

std::shared_ptr<matchObj> _nfa_pattern::search(const std::u16string&) {
    return make_shared<matchObj>();
}

std::shared_ptr<std::vector<matchObj>> _nfa_pattern::findall(const std::u16string&) {
    return make_shared<vector<matchObj>>();
}



