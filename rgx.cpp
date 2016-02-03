#include "rgx.h"
#include "ast.h"
#include "astnode.h"
#include <stack>

using namespace rgx;
using namespace std;

shared_ptr<_pattern> rgx::compile(const string & re) {
    _ast ast(re);
    astTraversal(ast);
    if (ast._build_type == _ast::build_to_dfa) {
        return make_shared<_dfa_pattern>(ast);
    } else {
        return make_shared<_nfa_pattern>(ast);
    }
}

void rgx::astTraversal(const _ast& ast) {
    auto r = ast._root;
    stack<decltype(r)> s;    
    if (r) {
        s.push(r);
    } else {
        return;
    }
    while(!s.empty()) {
        auto e = s.top();
        s.pop();
        cout << e->toString();
        if (e->_right) {
            s.push(e->_right);
        }
        if (e->_left) {
            s.push(e->_left);
        }
    }

    cout << "+++++++++++++++++++++++++++++++-----------astTraversalEnd-------------++++++++++++++++++++++++++++++++++++++++++\n" << endl;
}
