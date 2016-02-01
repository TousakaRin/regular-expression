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

void rgx::pattern::err() {
    exit(-1);
}

void rgx::pattern::err(const string& msg) {
    cout << "\n----------------------------------------------------------\n"
        << msg 
        << "\n----------------------------------------------------------\n";
    err();
}

void rgx::pattern::traversal() {

}

/*===================================DFA_PATTERN===================================*/

dfa_pattern::dfa_pattern(const _ast&) {
}


std::shared_ptr<matchObj> dfa_pattern::match(const std::u16string&) {
    return make_shared<matchObj>();
}

std::shared_ptr<matchObj> dfa_pattern::search(const std::u16string&) {
    return make_shared<matchObj>();
}

std::shared_ptr<std::vector<matchObj>> dfa_pattern::findall(const std::u16string&) {
    return make_shared<vector<matchObj>>();
}


void rgx::dfa_pattern::traversal(){

}

/*===================================NFA_PATTERN===================================*/
nfa_pattern::nfa_pattern(const _ast& ast) {
    if (ast._build_type != _ast::build_to_nfa) {
        err("ast与pattern类型不一致，应为_build_to_nfa");
    }

    //使用非递归方法后序遍历ast来生成NFA
    stack<shared_ptr<_astNode>> s;  
    auto p = ast._root;  
    do {  
        while(p) {  
            s.push(p);  
            p = p->left;  
        }  
        while(!s.empty() && (!s.top()->right || s.top()->right->nfa))  {  
            s.top()->generateNFA();
            s.pop();  
        }            
        if(!s.empty()) {  
            p = s.top()->right;  
        }  
    } while(!s.empty());  
    startNode = ast._root->nfa->first; 
    finishNode = ast._root->nfa->second;
}

std::shared_ptr<matchObj> nfa_pattern::match(const std::u16string&) {
    return make_shared<matchObj>();
}

std::shared_ptr<matchObj> nfa_pattern::search(const std::u16string&) {
    return make_shared<matchObj>();
}

std::shared_ptr<std::vector<matchObj>> nfa_pattern::findall(const std::u16string&) {
    return make_shared<vector<matchObj>>();
}


void rgx::nfa_pattern::traversal() {
    deque<shared_ptr<_NFA_Node>> dq;
    set<_NFA_Node*> s;
    dq.push_back(startNode);
    while(!dq.empty()) {
        s.insert(dq.front().get());
        for (auto edge : dq.front()->edges) {
            cout << edge->toString() << endl;
            if (s.find(edge->toNode.get()) == s.end()) {
                dq.push_back(edge->toNode);
                s.insert(edge->toNode.get());
            }
        }
        dq.pop_front();
    }
}
