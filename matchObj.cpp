#include "matchObj.h"
#include "stringTools.h"
#include <iostream>
#include <string>

using namespace std;
using namespace rgx;

u16string rgx::matchObj::group(unsigned int i) {
    if (i >= _capVector.size() || _re.size() == 0) {
        return u16string();
    }
    return _re.substr(_capVector[i].first, _capVector[i].second - _capVector[i].first);
}

void rgx::matchObj::justToTest(const std::u16string& s) {
    cout << "_capVector size: " << _capVector.size() << endl;
    for (unsigned int i = 1; i < _capVector.size(); ++i) {
        cout << "index : " << _capVector[i].first << " "<< _capVector[i].second << " " << ucs2_to_string(s.substr(_capVector[i].first, _capVector[i].second - _capVector[i].first)) << endl;
    }
}

vector<u16string> rgx::matchObj::groups() {
    vector<u16string> grps(_capVector.size(), u16string());
    if (_re.size() != 0) {
        for (unsigned int i = 0; i < _capVector.size(); ++i) {
            grps[i] = _input.substr(_capVector[i].first, _capVector[i].second - _capVector[i].first); 
        }
    }
    return grps;
}

pair<unsigned int, unsigned int> rgx::matchObj::span(unsigned int) {
    return pair<unsigned int, unsigned int> (_pos, _endpos);
}

u16string rgx::matchObj::group(const std::string& groupname) {
	return group((*_nameMap)[string_to_ucs2(groupname)]);
}


//在回溯算法运行时，不再拷贝re和input，仅仅在成功返回matchObj时，通过这个函数将re 和input加到match里边
void rgx::matchObj::addReAndInput(const u16string& re, const u16string& input, const unique_ptr<map<u16string, unsigned int>>& namemap) {
    _re = re;
    _input = input;
	_nameMap.reset(new map<u16string, unsigned int>(namemap->begin(), namemap->end()));
}
