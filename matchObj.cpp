#include "matchObj.h"
#include "stringTools.h"
#include <iostream>
#include <string>

using namespace std;
using namespace rgx;

vector<wstring> rgx::matchObj::group(unsigned int) {
    return vector<wstring>();
}

void rgx::matchObj::justToTest(const std::u16string& s) {
    cout << "wtf? " << _capVector.size() << endl;
    for (unsigned int i = 1; i < _capVector.size(); ++i) {
        cout << ucs2_to_string(s.substr(_capVector[i].first, _capVector[i].second - _capVector[i].first)) << endl;
    }
}

vector<vector<wstring>> rgx::matchObj::groups() {
    return vector<vector<wstring>>();
}

pair<unsigned int, unsigned int> rgx::matchObj::span(unsigned int) {
    return pair<unsigned int, unsigned int> (0,0);
}

vector<wstring> rgx::matchObj::group(const wstring &) {
    return vector<wstring>();
}

void rgx::matchObj::clear() {
    
}
