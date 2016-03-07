#include "ast.h"
#include <iostream>
#include "testCase.h"
#include "stringTools.h"
#include "rgx.h"

#define _ast_test_

using namespace std;
using namespace rgx;

int main() {
    cout << "\n\n++++++++++++-------------------------测试程序-----------------------++++++++++++\n" << endl;

#ifdef _ast_test_
    for (auto &s : testCase) {
        cout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n" << s << endl;
        auto ast(s);
        auto pattern = compile(s);
        pattern->traversal();
        u16string str = string_to_ucs2("abcaabc");
        auto mb = pattern->match(str);
        mb->justToTest(str);
    }
#endif

    return 0;
}
