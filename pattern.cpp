#include "pattern.h"

using namespace std;
using namespace rgx;


/*===================================DFA_PATTERN===================================*/

dfa_pattern::dfa_pattern(const _ast&) {

}


std::shared_ptr<matchObj> dfa_pattern::match(const std::wstring&) {
    return make_shared<matchObj>();
}

std::shared_ptr<matchObj> dfa_pattern::search(const std::wstring&) {
    return make_shared<matchObj>();
}

std::shared_ptr<std::vector<matchObj>> dfa_pattern::findall(const std::wstring&) {
    return make_shared<vector<matchObj>>();
}


/*===================================NFA_PATTERN===================================*/
nfa_pattern::nfa_pattern(const _ast&) {

}

std::shared_ptr<matchObj> nfa_pattern::match(const std::wstring&) {
    return make_shared<matchObj>();
}

std::shared_ptr<matchObj> nfa_pattern::search(const std::wstring&) {
    return make_shared<matchObj>();
}

std::shared_ptr<std::vector<matchObj>> nfa_pattern::findall(const std::wstring&) {
    return make_shared<vector<matchObj>>();
}

