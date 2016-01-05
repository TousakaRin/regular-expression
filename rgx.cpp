#include "rgx.h"
#include "ast.h"

using namespace rgx;
using namespace std;

shared_ptr<pattern> compile(const wstring & re) {
    _ast ast(re);
    if (ast.build_type == _ast::build_to_dfa) {
        return make_shared<dfa_pattern>(ast);
    } else {
        return make_shared<nfa_pattern>(ast);
    }
}
