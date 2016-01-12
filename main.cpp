#include "ast.h"
#include <iostream>
#include "stringTools.h"

#define _ast_test_

using namespace std;
using namespace rgx;

int main() {
    cout << "\n\n++++++++++++-------------无敌叼炸天的测试程序-------------++++++++++++\n" << endl;

#ifdef _ast_test_
//    wstring s = L"(?<!反向预读君)(?:括号君|(?P<捕获君>WTF?)){45-78}(?=预读君)连接君(?P=捕获君)";
    wstring s = L"(((?P<抓你啊>^卧槽？){9-18}?))";
    cout << wstring_to_utf8(s) << endl;
    _ast t(s);
    astTraversal(t);
    
#endif

    return 0;
}
