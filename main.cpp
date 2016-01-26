#include "ast.h"
#include <iostream>
#include "stringTools.h"

#define _ast_test_

using namespace std;
using namespace rgx;

int main() {
    cout << "\n\n++++++++++++-------------无敌叼炸天的测试程序-------------++++++++++++\n" << endl;

#ifdef _ast_test_

//     string s = L"(?<!反向预读君)(?:括号君|(?P<捕获君>WTF?)){45-78}(?=预读君)连接君(?P=捕获君)";
//     string s = u8"(?<!nozuonodie)(?P<抓你啊>^卧槽？){9-18}?(?!whyyoutry)";
    string s = u8"[123456\\W]?";
    _ast t(s);
    cout << s << endl;
    astTraversal(t);
    
#endif

    return 0;
}
