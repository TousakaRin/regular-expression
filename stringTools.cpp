#include "stringTools.h"
#include <algorithm>

using namespace rgx;
using namespace std;

string rgx::int2string(int i) {
    string s;
    if (i == 0) {
        return "0";
    }
    if (i < 0) {
        i = -i;
        s.push_back('-');
    }
    string tmp;
    while(i != 0) {
        tmp.push_back(i % 10);
        i /= 10;
    }
    reverse(tmp.begin(), tmp.end());
    s += tmp;
    return s;
} 

string rgx::bool2string(bool b) {
    if (b) {
        return "true";
    } else {
        return "false";
    }
}
