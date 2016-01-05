#include "matchObj.h"

using namespace rgx;
using namespace std;

vector<wstring> matchObj::group(unsigned int) {
    return vector<wstring>();
}

vector<vector<wstring>> matchObj::groups() {
    return vector<vector<wstring>>();
}

pair<unsigned int, unsigned int> span(unsigned int) {
    return pair<unsigned int, unsigned int> (0,0);
}

vector<wstring> matchObj::group(const wstring &) {
    return vector<wstring>();
}
