#include "matchObj.h"

using namespace rgx;
using namespace std;

vector<wstring> rgx::matchObj::group(unsigned int) {
    return vector<wstring>();
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
