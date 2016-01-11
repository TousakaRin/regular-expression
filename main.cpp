#include "ast.h"
#include <iostream>

#define _ast_test_

using namespace std;
using namespace rgx;

int main() {
    cout << "无敌叼炸天的测试程序" << endl;

#ifdef _ast_test_
    wstring s = L"(?<!反向预读君)(?:括号君|(?P<捕获君>WTF?)){45-78}(?=预读君)";
    _ast t(s);
    astTraversal(t);
    
#endif

    return 0;
}
