#include <stack>
#include <set>
#include <deque>
#include "pattern.h"
#include "cstdlib"
#include "ast.h"
#include "astnode.h"
#include "nfaEdge.h"
#include "nfaNode.h"

using namespace std;
using namespace rgx;


/*=================================PATTERN=========================================*/

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

}

/*===================================DFA_PATTERN===================================*/

_dfa_pattern::_dfa_pattern(const _ast& ast) : _edgeMgr(ast._edgeMgr) {
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


void rgx::_dfa_pattern::traversal(){

}

/*===================================NFA_PATTERN===================================*/

_nfa_pattern::_nfa_pattern(const _ast& ast) : _edgeMgr(ast._edgeMgr) {
    if (ast._build_type != _ast::build_to_nfa) {
        err("ast与pattern类型不一致，应为_build_to_nfa");
    }

    //使用非递归方法后序遍历ast来生成NFA
    stack<shared_ptr<_astNode>> s;  
    auto p = ast._root;  
    do {  
        while(p) {  
            s.push(p);  
            p = p->_left;  
        }  
        while(!s.empty() && (!s.top()->_right || s.top()->_right->_nfa))  {  
            s.top()->generateNFA();
            s.pop();  
        }            
        if(!s.empty()) {  
            p = s.top()->_right;  
        }  
    } while(!s.empty());  
    startNode = ast._root->_nfa->first; 
    finishNode = ast._root->_nfa->second;
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


void rgx::_nfa_pattern::traversal() {
    deque<shared_ptr<_NFA_Node>> dq;
    set<_NFA_Node*> s;
    dq.push_back(startNode);
    while(!dq.empty()) {
        s.insert(dq.front().get());
        for (auto edge : dq.front()->edges) {
            cout << edge->toString() << endl;
            if (s.find(edge->_toNode.get()) == s.end()) {
                dq.push_back(edge->_toNode);
                s.insert(edge->_toNode.get());
            }
        }
        dq.pop_front();
    }
}
