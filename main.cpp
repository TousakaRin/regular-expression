#include "ast.h"
#include <iostream>
#include "testCase.h"
#include "stringTools.h"
#include "rgx.h"

#define _ast_test_

using namespace std;
using namespace rgx;

int main() {
    cout << "\n\n++++++++++++-------------无敌叼炸天的测试程序-------------++++++++++++\n" << endl;

#ifdef _ast_test_
    for (auto &s : testCase) {
        cout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n" << s << endl;
        auto pattern = compile(s);
        pattern->traversal();
    }
#endif

    return 0;
}
