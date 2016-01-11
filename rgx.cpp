#include "rgx.h"
#include "ast.h"
#include <stack>

using namespace rgx;
using namespace std;

shared_ptr<pattern> rgx::compile(const wstring & re) {
    _ast ast(re);
    if (ast.build_type == _ast::build_to_dfa) {
        return make_shared<dfa_pattern>(ast);
    } else {
        return make_shared<nfa_pattern>(ast);
    }
}

void rgx::astTraversal(const _ast& ast) {
    //auto r = ast.root;
    //cout << r->toString();
    //if (r->left) {
    //    cout << r->left->toString() << endl;
    //}
    //if (r->right) {
    //    cout << r->right->toString() << endl;
    //}
    auto r = ast.root;
    stack<decltype(r)> s;    
    if (r) {
        s.push(r);
    } else {
        cout << "r == nullptr !" << endl;
        return;
    }
    while(!s.empty()) {
        auto e = s.top();
        s.pop();
        cout << e->toString();
        if (e->right) {
            s.push(e->right);
        }
        if (e->left) {
            s.push(e->left);
        }
    }
}
