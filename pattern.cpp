#include "pattern.h"
#include "cstdlib"
#include "ast.h"
#include "astnode.h"

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
    auto nfa = ast._root->generateNFA();
    startNode = nfa->first;
    finishNode = nfa->second;
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
}
